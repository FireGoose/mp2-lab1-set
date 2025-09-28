#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
  if (len >= 0)
    bitLen = len;
  else
    throw(length_error("Invalid length"));
  
  memLen = (bitLen + NBITS - 1) / NBITS;
  pMem = new TELEM[len];

  for (int i = 0; i < memLen; i++)
    pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
  bitLen = bf.bitLen;
  memLen = bf.memLen;
  if (memLen > 0)
    pMem = new TELEM[bitLen];

  for (int i = 0; i < memLen; ++i)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  if (pMem != nullptr)
  {
    delete[] pMem;
    pMem = nullptr;
  }
  bitLen = 0;
  memLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if (n > bitLen - 1 || n < 0) 
    throw(out_of_range("Index out of range"));
  
  return n / (NBITS);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if (n > bitLen - 1 || n < 0) 
    throw(out_of_range("Index out of range"));
  
  return (1 << (n % NBITS));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return bitLen; 
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n > bitLen - 1 || n < 0)
    throw(out_of_range("Index out of range"));
  
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n > bitLen - 1 || n < 0)
    throw(out_of_range("Index out of range"));
  
  pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{

  if (n > bitLen - 1 || n < 0)
    throw(out_of_range("Index out of range"));

  return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
  if (&bf != this)
  {
    if (pMem != nullptr)
      delete[] pMem;

    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new TELEM[bitLen];
    for (int i = 0; i < memLen; ++i) 
      pMem[i] = bf.pMem[i];

  }

  return *this;
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
  if (memLen != bf.memLen) 
    return false;
  
  for (int i = 0; i < memLen; ++i) 
  {
    if (pMem[i] != bf.pMem[i]) 
      return false;
  }
  
  return true;
}

bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
  int maxLen = std::max(bitLen, bf.bitLen);
  TBitField newBitField(maxLen);
  for (int i = 0; i < memLen; ++i)
    newBitField.pMem[i] |= pMem[i];

  TBitField res(bitLen);
  for (int i = 0; i < bf.memLen; ++i)
    newBitField.pMem[i] |= bf.pMem[i];

  return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
  int maxBitLen = std::max(bitLen, bf.bitLen);
  int minMemLen = std::max(memLen, bf.memLen);
  TBitField newBitField(maxBitLen);

  TBitField res(bitLen);
  for (int i = 0; i < minMemLen; ++i)
    res.pMem[i] &= bf.pMem[i];

  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField res(bitLen);
  for (int i = 0; i < memLen; ++i)
    res.pMem[i] = ~pMem[i];

  return res;
}

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    for (size_t i = 0; i < bf.GetLength(); ++i) 
    {
      int tmp;
      istr >> tmp;
      
      if (tmp == 0)
        bf.ClrBit(i);
      else if (tmp == 1)
        bf.SetBit(i);
      else
        throw std::invalid_argument("Bit value should be 0 or 1");
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
  for (int i = 0; i < bf.GetLength(); ++i) 
    ostr << bf.GetBit(i) << ", ";

  return ostr;
}
