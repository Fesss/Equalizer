// Simple PCM RIFF WAVE support

#ifndef _RIFF_
#define _RIFF_

#include <stdio.h>

#include "cdefs.h"

// ��������� ������

// ��� �������-����������� ������.
typedef void (*riff_error_t)( const char* msg );

// ��������� ����������� ������.
// �� ��������� ���������� ������ ������� ��������� �� stdout
// � ��������� ���������� ���������.
void setup_riff_error( riff_error_t );

// ������ ����� ������� wav

class riffwave_reader {
public:
  // �������� bufsize ������ ������ ������ ��� �������� �������������� ������ ������.
  // ���� �������� ����� ��������� ����� 0 (�� ���������), �� ��� ������ �������� �����
  // � ������ ������ ��������� � ����������� �������� � ����� (��. data_size()).
  riffwave_reader( const char* fname, int bufsize = 0 );

  ~riffwave_reader();

  int channels() const { return _channels; }
    // ��������� ���������� ������ (����/������)
  int samplespersec() const { return _samplespersec; }
    // ��������� ���������� �������� � �������
  int bitspersample() const { return _bitspersample; }
    // ��������� ���������� ����� �� ���� ������

  int data_size() const { return _datasize / _alignment; }
    // ��������� ���������� ��������

  // ��������� ��� ������ � ������� �������
  // ��� �������������� ������ ���������, ��� ������������ ������ ����� �����
  enum channel { LEFT, RIGHT };

  // ��������� ������ ������� �� ��������� ������
  // ������� ��������� ������ ������� j ������������
  // �������� ������, �������� ��� ���������������.
  int operator()( int j, channel ch = LEFT ) const;

  // ���������������� ������ �� �������� ����� � �����
  void seek( int pos );
  // ��������� ������� ������� ������
  int tell() const { return _bufpos; }
  // ��������� ������� ������
  int buf_size() const { return _buflen; }

private:
  FILE* _file;

  int _channels;
  int _samplespersec; // � ����� ������
  int _bitspersample; // � ����� ������
  int _alignment;     // ������ �� ���� ������ ����� �������
  int _datasize;      // ����� ������ � ������
  int _datapos;       // ������� ������ ������ � ����� � ������

  int _bufpos;           // ������� ������� ������ � ��������
  int _buflen;           // ������ ����������� ����� ������
  int _bufsize;          // ������ ������ ������ � ������
  uchar* _data;

  int wait4data();
  int wait4fmt();

  // ��������� ���������� ������
  riffwave_reader( const riffwave_reader& );
  riffwave_reader& operator=( const riffwave_reader& );
};

// ����� ������� data ������ len � ������������� 16-������ wav � ������ fname.
// �������� ������ ������ ������������ ����� ������������ ����� �� ��������� [-1.0, 1.0].
// sps - ������� ������������� � ������ (���������� �������� � �������).
void save_as_riff( const char* fname, const double* data, int len, int sps );

#endif // _RIFF_
