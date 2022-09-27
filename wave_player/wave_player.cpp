//-----------------------------------------------------------------------------
// a sample mbed library to play back wave files.
//
// explanation of wave file format.
// https://ccrma.stanford.edu/courses/422/projects/WaveFormat/

// if VERBOSE is uncommented then the wave player will enter a verbose
// mode that displays all data values as it reads them from the file
// and writes them to the DAC.  Very slow and unusable output on the DAC,
// but useful for debugging wave files that don't work.
//#define VERBOSE


#include <mbed.h>
#include <stdio.h>
#include <wave_player.h>


//-----------------------------------------------------------------------------
// constructor -- accepts an mbed pin to use for AnalogOut.  Only p18 will work
wave_player::wave_player(AnalogOut *_dac)
{
  wave_DAC=_dac;
  wave_DAC->write_u16(32768);        //DAC is 0-3.3V, so idles at ~1.6V
  verbosity=0;
}

//-----------------------------------------------------------------------------
// if verbosity is set then wave player enters a mode where the wave file
// is decoded and displayed to the screen, including sample values put into
// the DAC FIFO, and values read out of the DAC FIFO by the ISR.  The DAC output
// itself is so slow as to be unusable, but this might be handy for debugging
// wave files that don't play
//-----------------------------------------------------------------------------
void wave_player::set_verbosity(int v)
{
  verbosity=v;
}

//-----------------------------------------------------------------------------
// player function.  Takes a pointer to an opened wave file.  The file needs
// to be stored in a filesystem with enough bandwidth to feed the wave data.
// LocalFileSystem isn't, but the SDcard is, at least for 22kHz files.  The
// SDcard filesystem can be hotrodded by increasing the SPI frequency it uses
// internally.
//-----------------------------------------------------------------------------
void wave_player::play(FILE *wavefile)
{
        unsigned chunk_id,chunk_size,channel;
        unsigned data,samp_int,i;
        short unsigned dac_data;
        long long slice_value;
        char *slice_buf;
        short *data_sptr;
        unsigned char *data_bptr;
        int *data_wptr;
        FMT_STRUCT wav_format;
        long slice,num_slices;
  DAC_wptr=0;
  DAC_rptr=0;
  for (i=0;i<256;i+=2) {
    DAC_fifo[i]=0;
    DAC_fifo[i+1]=3000;
  }
  DAC_wptr=4;
  DAC_on=0;

  fread(&chunk_id,4,1,wavefile);
  fread(&chunk_size,4,1,wavefile);
  while (!feof(wavefile)) {
    if (verbosity)
      printf("Read chunk ID 0x%x, size 0x%x\n",chunk_id,chunk_size);
    switch (chunk_id) {
      case 0x46464952:
        fread(&data,4,1,wavefile);
        if (verbosity) {
          printf("RIFF chunk\n");
          printf("  chunk size %d (0x%x)\n",chunk_size,chunk_size);
          printf("  RIFF type 0x%x\n",data);
        }
        break;
      case 0x20746d66:
        fread(&wav_format,sizeof(wav_format),1,wavefile);
        if (verbosity) {
          printf("FORMAT chunk\n");
          printf("  chunk size %d (0x%x)\n",chunk_size,chunk_size);
          printf("  compression code %d\n",wav_format.comp_code);
          printf("  %d channels\n",wav_format.num_channels);
          printf("  %d samples/sec\n",wav_format.sample_rate);
          printf("  %d bytes/sec\n",wav_format.avg_Bps);
          printf("  block align %d\n",wav_format.block_align);
          printf("  %d bits per sample\n",wav_format.sig_bps);
        }
        if (chunk_size > sizeof(wav_format))
          fseek(wavefile,chunk_size-sizeof(wav_format),SEEK_CUR);
        break;
      case 0x61746164:
// allocate a buffer big enough to hold a slice
        slice_buf=(char *)malloc(wav_format.block_align);
        if (!slice_buf) {
          printf("Unable to malloc slice buffer");
          exit(1);
        }
        num_slices=chunk_size/wav_format.block_align;
        samp_int=1000000/(wav_format.sample_rate);
        if (verbosity) {
          printf("DATA chunk\n");
          printf("  chunk size %d (0x%x)\n",chunk_size,chunk_size);
          printf("  %d slices\n",num_slices);
          printf("  Ideal sample interval=%d\n",(unsigned)(1000000.0/wav_format.sample_rate));
          printf("  programmed interrupt tick interval=%d\n",samp_int);
        }

// starting up ticker to write samples out -- no printfs until tick.detach is called
        if (verbosity)
          tick.attach_us(this,&wave_player::dac_out, 500000); 
        else
          tick.attach_us(this,&wave_player::dac_out, samp_int); 
        DAC_on=1; 

// start reading slices, which contain one sample each for however many channels
// are in the wave file.  one channel=mono, two channels=stereo, etc.  Since
// mbed only has a single AnalogOut, all of the channels present are averaged
// to produce a single sample value.  This summing and averaging happens in
// a variable of type signed long long, to make sure that the data doesn't
// overflow regardless of sample size (8 bits, 16 bits, 32 bits).
//
// note that from what I can find that 8 bit wave files use unsigned data,
// while 16 and 32 bit wave files use signed data
//
        for (slice=0;slice<num_slices;slice+=1) {
          fread(slice_buf,wav_format.block_align,1,wavefile);
          if (feof(wavefile)) {
            printf("Oops -- not enough slices in the wave file\n");
            exit(1);
          }
          data_sptr=(short *)slice_buf;     // 16 bit samples
          data_bptr=(unsigned char *)slice_buf;     // 8 bit samples
          data_wptr=(int *)slice_buf;     // 32 bit samples
          slice_value=0;
          for (channel=0;channel<wav_format.num_channels;channel++) {
            switch (wav_format.sig_bps) {
              case 16:
                if (verbosity)
                  printf("16 bit channel %d data=%d ",channel,data_sptr[channel]);
                slice_value+=data_sptr[channel];
                break;
              case 32:
                if (verbosity)
                  printf("32 bit channel %d data=%d ",channel,data_wptr[channel]);
                slice_value+=data_wptr[channel];
                break;
              case 8:
                if (verbosity)
                  printf("8 bit channel %d data=%d ",channel,(int)data_bptr[channel]);
                slice_value+=data_bptr[channel];
                break;
            }
          }
          slice_value/=wav_format.num_channels;
          
// slice_value is now averaged.  Next it needs to be scaled to an unsigned 16 bit value
// with DC offset so it can be written to the DAC.
          switch (wav_format.sig_bps) {
            case 8:     slice_value<<=8;
                        break;
            case 16:    slice_value+=32768;
                        break;
            case 32:    slice_value>>=16;
                        slice_value+=32768;
                        break;
          }
          dac_data=(short unsigned)slice_value;
          if (verbosity)
            printf("sample %d wptr %d slice_value %d dac_data %u\n",slice,DAC_wptr,(int)slice_value,dac_data);
          DAC_fifo[DAC_wptr]=dac_data;
          DAC_wptr=(DAC_wptr+1) & 0xff;
          while (DAC_wptr==DAC_rptr) {
          }
        }
        DAC_on=0;
        tick.detach();
        free(slice_buf);
        break;
      case 0x5453494c:
        if (verbosity)
          printf("INFO chunk, size %d\n",chunk_size);
        fseek(wavefile,chunk_size,SEEK_CUR);
        break;
      default:
        printf("unknown chunk type 0x%x, size %d\n",chunk_id,chunk_size);
        data=fseek(wavefile,chunk_size,SEEK_CUR);
        break;
    }
    fread(&chunk_id,4,1,wavefile);
    fread(&chunk_size,4,1,wavefile);
  }
}


void wave_player::dac_out()
{
  if (DAC_on) {
#ifdef VERBOSE
  printf("ISR rdptr %d got %u\n",DAC_rptr,DAC_fifo[DAC_rptr]);
#endif
    wave_DAC->write_u16(DAC_fifo[DAC_rptr]);
    DAC_rptr=(DAC_rptr+1) & 0xff;
  }
}

