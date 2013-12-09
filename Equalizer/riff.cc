#include "riff.h"

#include <stdlib.h>
#include <string.h>

// error handler

namespace {
  void std_riff_error( const char* msg )
  {
    fprintf(stderr, "riff: error: %s\n", msg);
    exit(1);
  }

  riff_error_t riff_error = std_riff_error;
}

void setup_riff_error_handler( riff_error_t handler )
{
  riff_error = handler;
}

// definitions

const char RIFFCHUNK_ID[] = "RIFF";  // идентификатор чанка RIFF
const char FMTCHUNK_ID[]  = "fmt ";  // идентификатор чанка fmt
const char DATACHUNK_ID[] = "data";  // идентификатор чанка data

const int CHUNKID_SIZE  = 4;  // размер идентификатора чанка
const int CHUNKHDR_SIZE = 8;  // размер заголовка чанка

const char WAVEFORM_ID[]  = "WAVE"; // инентификатор WAVE-формы
const int WAVEFORMID_SIZE = 4;      // размер инентификатора WAVE-формы

const int PCMFORMAT_ID      = 1;   // идентификатор PCM-формата
const int FMTCHUNK_DATASIZE = 16;  // размер данных чанка fmt
const int FMTCHUNK_SIZE = CHUNKHDR_SIZE + FMTCHUNK_DATASIZE; // (заголовок + данные)

// out parameters

const int OUT_CHANNELS       = 1;  // число каналов при выводе
const int OUT_BYTESPERSAMPLE = 2;  // байт на отсчет при выводе
const int OUT_BITSPERSAMPLE  =  OUT_BYTESPERSAMPLE * 8;
const int OUT_ALIGNMENT = OUT_CHANNELS * OUT_BYTESPERSAMPLE; // число байтов на отсчет (во всех каналах)

inline int OUT_BYTESPERSEC( int sps )
{
  return OUT_CHANNELS * sps * OUT_BYTESPERSAMPLE;
}

inline int DATA_SIZE( int size )
{
  return OUT_CHANNELS * size * OUT_BYTESPERSAMPLE;
}

inline int DATACHUNK_SIZE( int size )
{
  return CHUNKHDR_SIZE + DATA_SIZE(size);
}

// common

inline int ALIGN2EVEN( int size )
{
  return (size & 0x01) ? size + 1 : size;
}

inline bool ISODD( int size )
{
  return size & 0x01;
}

// параметры сигнала

const int PCM8_MIN = 0;
const int PCM8_MAX = 255;
const int PCM8_MID = 128;

const int PCM16_MIN = -32768;
const int PCM16_MAX = 32767;
const int PCM16_MID = 0;

// chunk header

class chunkhdr {
public:
  chunkhdr() {}
  chunkhdr( FILE* file ) { read(file); }

  void read( FILE* );

  void skipdata( FILE* );

  bool is_fmt() const  { return isid(FMTCHUNK_ID); }
  bool is_data() const { return isid(DATACHUNK_ID); }
  bool is_riff() const { return isid(RIFFCHUNK_ID); }
  int  size() const { return _sz; }

private:
  char   _id[CHUNKID_SIZE];  // идентификатор чанка
  uint32 _sz;                // размер данных в байтах

  bool isid( const char* id ) const;
};

void chunkhdr::read( FILE* file )
{
  if( fread(_id, sizeof(_id), 1, file) != 1 )
    riff_error("can't read chunk header from file");
  if( fread(&_sz, sizeof(_sz), 1, file) != 1 )
    riff_error("can't read chunk header from file");
}

void chunkhdr::skipdata( FILE* file )
{
  if( fseek(file, _sz, SEEK_CUR) != 0 )
    riff_error("can't skip chunk data in file");
}

bool chunkhdr::isid( const char *id ) const
{
  return strncmp(_id, id, CHUNKID_SIZE) == 0;
}

// WAVE-form hdr

class waveformhdr {
public:
  waveformhdr( FILE* );

  bool is_wave() const;

private:
  char _id[WAVEFORMID_SIZE];
};

waveformhdr::waveformhdr( FILE* file )
{
  if( fread(&_id, WAVEFORMID_SIZE, 1, file) != 1 )
    riff_error("can't read waveform header from file");
}

bool waveformhdr::is_wave() const
{
  return strncmp(_id, WAVEFORM_ID, WAVEFORMID_SIZE) == 0;
}

// fmt-chunk

class fmtchunk {
public:
  fmtchunk( FILE* file, int size );

  int channels() const { return _channels; }
  int samplespersec() const { return _samplespersec; }
  int bitspersample() const { return _bitspersample; }
  int alignment() const { return _alignment; }

private:
  uint16 _formattag;         // категория формата
  uint16 _channels;          // число каналов
  uint16  _bitspersample;    // разрядность сампла (в одном канале)
  uint32 _samplespersec;     // частота дискретизации (самплов в секунду в одном канале)
  uint32 _bytespersec;       // число байт в секунду для всех каналов
  uint16 _alignment;         // выравнивание данных
  // _bytespersec = (_channels * _samplespersec * _bitspersample)/8
  // _alignment = (nChannels * nBitsPerSample)/8
};

fmtchunk::fmtchunk( FILE* file, int size )
{
  if( size < FMTCHUNK_DATASIZE )
    riff_error("incorrect data size in fmt-chunk");

  if( fread(&_formattag, sizeof(_formattag), 1, file) != 1 )
    riff_error("can't read format chunk from file");
  if( fread(&_channels, sizeof(_channels), 1, file) != 1 )
    riff_error("can't read format chunk from file");
  if( fread(&_samplespersec, sizeof(_samplespersec), 1, file) != 1 )
    riff_error("can't read format chunk from file");
  if( fread(&_bytespersec, sizeof(_bytespersec), 1, file) != 1 )
    riff_error("can't read format chunk from file");
  if( fread(&_alignment, sizeof(_alignment), 1, file) != 1 )
    riff_error("can't read format chunk from file");
  if( fread(&_bitspersample, sizeof(_bitspersample), 1, file) != 1 )
    riff_error("can't read format chunk from file");

  if( _formattag != PCMFORMAT_ID )
    riff_error("file format seems not to be a PCM");

  if( size > FMTCHUNK_DATASIZE ){
    if( fseek(file, size-FMTCHUNK_DATASIZE, SEEK_CUR) != 0 )
      riff_error("can't skip format chunk data in file");
  }
}

// riff wave reader

int riffwave_reader::wait4data()
{
  chunkhdr chh;
  while( 1 ){
    chh.read(_file);
    if( chh.is_data() )
      break;
    else
      chh.skipdata(_file);
  }
  return chh.size();
}

int riffwave_reader::wait4fmt()
{
  chunkhdr chh;
  while( 1 ){
    chh.read(_file);
    if( chh.is_fmt() )
      break;
    else
      chh.skipdata(_file);
  }
  return chh.size();
}

void riffwave_reader::seek( int pos )
{
  if( pos < 0 || pos > data_size()-1 )
    riff_error("position out of bounds");
  _bufpos = pos;
  _buflen = (data_size() - pos < _bufsize / _alignment) ? data_size() - pos : _bufsize / _alignment;
  if( fseek(_file, _datapos + pos * _alignment, SEEK_SET) != 0 )
    riff_error("can't seek in file");
  if( fread(_data, _alignment, _buflen, _file) != static_cast<uint>(_buflen) )
    riff_error("can't read data from file");
}

riffwave_reader::riffwave_reader( const char *fname, int bufsize )
{
  // открыть файл
  _file = fopen(fname, "rb");
  if( _file == 0 )
    riff_error("can't open file");

  // проверить чанк RIFF
  chunkhdr chh(_file);
  if( !chh.is_riff() )
    riff_error("bad riff-chunk");

  // проверить чанк WAVE
  waveformhdr wfh(_file);
  if( !wfh.is_wave() )
    riff_error("bad wave-chunk");

  // дождаться чанка fmt и прочитать
  int fmtsize = wait4fmt();
  fmtchunk fmt(_file, fmtsize);
  _channels = fmt.channels();
  _samplespersec = fmt.samplespersec();
  _bitspersample = fmt.bitspersample();
  if( _bitspersample != 8 && _bitspersample != 16 )
    riff_error("RIFF WAVE file with unsupported bitspersample (may be 8 or 16)");
  _alignment = fmt.alignment();
  if( _alignment * 8 != _channels * _bitspersample )
    riff_error("incorrect value of alignment field in RIFF fmt chunk");

  // дождаться данных и прочитать
  _datasize = wait4data();
  _datapos = ftell(_file);
  if( _datasize % _alignment != 0 )
    riff_error("incorrect data size in RIFF data chunk");

  if( bufsize > 0 ) // задан размер буфера
    _bufsize = bufsize * _alignment;
  else // буфер вмещает все данные из файла
    _bufsize = _datasize;
  _data = static_cast<uchar*>(malloc(_bufsize));
  if( _data == 0 )
    riff_error("can't allocate memory buffer");

  seek(0);
}

riffwave_reader::~riffwave_reader()
{
  if( fclose(_file) != 0 )
    riff_error("can't close file");
  free(_data);
}

int riffwave_reader::operator()( int j, channel ch ) const
{
  if( j < 0 || j > buf_size()-1 )
    riff_error("index out of bounds");
  if( ch == RIGHT && _channels != 2 )
    riff_error("can't read right channel for mono RIFF WAVE");

  int sample = j * _alignment;
  if( ch == RIGHT ) sample += (_bitspersample == 8) ? 1 : 2;
  uint16 buf = (_bitspersample == 8) ? _data[sample] : (_data[sample+1] << 8) | _data[sample];
  return static_cast<int16>(buf);
}

// riff wave saver

namespace {
  void write_chunkhdr( FILE* file, const char *id, uint32 sz )
  {
    if( fwrite(id, CHUNKID_SIZE, 1, file) != 1 )
      riff_error("can't write chunk header to file");
    if( fwrite(&sz, sizeof(uint32), 1, file) != 1 )
      riff_error("can't write chunk size to file");
  }

  void write_fmt( FILE* file, int sps )
  {
    uint16 formattag = PCMFORMAT_ID;           // категори формата
    uint16 channels = OUT_CHANNELS;            // число каналов
    uint32 samplespersec = sps;                // частота дискретизации
    uint16 bitspersample = OUT_BITSPERSAMPLE;  // разрдность дискретизации
    uint32 bytespersec = OUT_BYTESPERSEC(sps); // число байт в секунду
    uint16 alignment = OUT_ALIGNMENT;          // выравнивание данных

    if( fwrite(&formattag, sizeof(formattag), 1, file) != 1 )
      riff_error("can't write waveform to file");
    if( fwrite(&channels, sizeof(channels), 1, file) != 1 )
      riff_error("can't write waveform to file");
    if( fwrite(&samplespersec, sizeof(samplespersec), 1, file) != 1 )
      riff_error("can't write waveform to file");
    if( fwrite(&bytespersec, sizeof(bytespersec), 1, file) != 1 )
      riff_error("can't write waveform to file");
    if( fwrite(&alignment, sizeof(alignment), 1, file) != 1 )
      riff_error("can't write waveform to file");
    if( fwrite(&bitspersample, sizeof(bitspersample), 1, file) != 1 )
      riff_error("can't write waveform to file");
  }

  void write_data( FILE* file, const double* data, int len )
  {
    for( int j = 0; j < len; j++ ){
      double val = data[j];
      if( val < -1.0 )
        val = -1.0;
      if( val > 1.0)
        val = 1.0;
      uint16 buf = static_cast<uint16>(val * PCM16_MAX);
      uchar lo = buf & 0x00FF;
      uchar hi = (buf >> 8) & 0x00FF;
      if( fwrite(&lo, sizeof(lo), 1, file) != 1 )
        riff_error("can't write data to file");
      if( fwrite(&hi, sizeof(hi), 1, file) != 1 )
        riff_error("can't write data to file");
    }
  }

  void write_align( FILE* file )
  {
    uchar buf = 0;
    if( fwrite(&buf, sizeof(uchar), 1, file) != 1 )
      riff_error("can't write alignment to file");
  }
}

void save_as_riff( const char* fname, const double* data, int len, int sps )
{
  FILE* file = fopen(fname, "wb");
  if( file == 0 )
    riff_error("can't create file");

  int riff_size = WAVEFORMID_SIZE + FMTCHUNK_SIZE + DATACHUNK_SIZE(len);
  // RIFF-chunk
  write_chunkhdr(file, RIFFCHUNK_ID, ALIGN2EVEN(riff_size));
  // WAVE-form
  if( fwrite(WAVEFORM_ID, WAVEFORMID_SIZE, 1, file) != 1 )
    riff_error("can't write waveform id to file");
  // fmt_chunk
  write_chunkhdr(file, FMTCHUNK_ID, FMTCHUNK_DATASIZE);
  write_fmt(file, sps);
  // data_chunk
  write_chunkhdr(file, DATACHUNK_ID, DATA_SIZE(len));
  write_data(file, data, len);
  // alignment
  if( ISODD(riff_size) ) 
    write_align(file);

  fclose(file);
}
