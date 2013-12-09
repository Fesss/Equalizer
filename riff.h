// Simple PCM RIFF WAVE support

#ifndef _RIFF_
#define _RIFF_

#include <stdio.h>

#include "cdefs.h"

// Обработка ошибок

// Тип функции-обработчика ошибок.
typedef void (*riff_error_t)( const char* msg );

// Установка обработчика ошибок.
// По умолчанию обработчик ошибок выводит сообщение на stdout
// и завершает выполнение программы.
void setup_riff_error( riff_error_t );

// Чтение файла формата wav

class riffwave_reader {
public:
  // Параметр bufsize задает размер буфера для варианта буфериованного чтения данных.
  // Если значение этого параметра равно 0 (по умолчанию), то все данные читаются сразу
  // и размер буфера совпадает с количеством отсчетов в файле (см. data_size()).
  riffwave_reader( const char* fname, int bufsize = 0 );

  ~riffwave_reader();

  int channels() const { return _channels; }
    // Получение количества канало (моно/стерео)
  int samplespersec() const { return _samplespersec; }
    // Получение количества отсчетов в секунду
  int bitspersample() const { return _bitspersample; }
    // Получение количества битов на один отсчет

  int data_size() const { return _datasize / _alignment; }
    // Получение количества отсчетов

  // Константы для левого и правого каналов
  // Для монофонической записи считается, что присутствует только левый канал
  enum channel { LEFT, RIGHT };

  // Получение одного отсчета из заданного канала
  // Границы изменения номера отсчета j определяются
  // размером буфера, заданным при конструировании.
  int operator()( int j, channel ch = LEFT ) const;

  // Позиционирование буфера на заданное место в файле
  void seek( int pos );
  // Получение текущей позиции буфера
  int tell() const { return _bufpos; }
  // Получение размера буфера
  int buf_size() const { return _buflen; }

private:
  FILE* _file;

  int _channels;
  int _samplespersec; // в одном канале
  int _bitspersample; // в одном канале
  int _alignment;     // байтов на один отсчет обоих каналов
  int _datasize;      // всего данных в байтах
  int _datapos;       // позиция начала данных в файле в байтах

  int _bufpos;           // текущая позиция буфера в отсчетах
  int _buflen;           // размер заполненной части буфера
  int _bufsize;          // полный размер буфера в байтах
  uchar* _data;

  int wait4data();
  int wait4fmt();

  // запрещаем копировать объект
  riffwave_reader( const riffwave_reader& );
  riffwave_reader& operator=( const riffwave_reader& );
};

// Вывод массива data длиной len в одноканальный 16-битный wav с именем fname.
// Исходные данные должны представлять собой вещественные числа из диапазона [-1.0, 1.0].
// sps - частота дискретизации в герцах (количество отсчетов в секунду).
void save_as_riff( const char* fname, const double* data, int len, int sps );

#endif // _RIFF_
