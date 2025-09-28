#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : maxPower(mp), bitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet& s) : maxPower(s.maxPower), bitField(s.bitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : maxPower(bf.GetLength()), bitField(bf)
{
}

TSet::operator TBitField()
{
  return bitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
  return maxPower;
}

int TSet::IsMember(const int elem) const // элемент множества?
{
  return bitField.GetBit(elem);
}

void TSet::InsElem(const int elem) // включение элемента множества
{
  bitField.SetBit(elem);
}

void TSet::DelElem(const int elem) // исключение элемента множества
{
  bitField.ClrBit(elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
  maxPower = s.maxPower;
  bitField = s.bitField;
  return *this;
}

bool TSet::operator==(const TSet& s) const // сравнение
{
  return bitField == s.bitField;
}

bool TSet::operator!=(const TSet& s) const // сравнение
{
  return bitField != s.bitField;
}

TSet TSet::operator+(const TSet& s) // объединение
{
  TSet tmp = bitField | s.bitField;
  return tmp;
}

TSet TSet::operator+(const int elem) // объединение с элементом
{
  TSet tmp = *this;
  tmp.InsElem(elem);
  return tmp;
}

TSet TSet::operator-(const int elem) // разность с элементом
{
  TSet tmp = *this;
  tmp.DelElem(elem);
  return tmp;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
  TSet tmp = bitField & s.bitField;
  return tmp;
}

TSet TSet::operator~(void) // дополнение
{
  TSet tmp(~bitField);
  return tmp;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
  istr >> s.bitField;
  return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
  ostr << s.bitField;
  return ostr;
}
