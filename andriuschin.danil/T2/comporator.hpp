#ifndef COMPORATOR_HPP
#define COMPORATOR_HPP

#include "data_struct.hpp"

namespace andriuschin
{
  struct Comporator
  {
    inline bool operator()(const DataStruct& lhs, const DataStruct& rhs) const
    {
      if (lhs.key1 != rhs.key1)
      {
        return lhs.key1 < rhs.key1;
      }
      else if (lhs.key2 != rhs.key2)
      {
        return (static_cast< double >(lhs.key2.first) / lhs.key2.second)
            < (static_cast< double >(rhs.key2.first) / rhs.key2.second);
      }
      else
      {
        return lhs.key3.size() < lhs.key3.size();
      }
    }
  };
}

#endif
