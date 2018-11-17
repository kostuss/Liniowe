#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <cstring>
#include <cmath>
#define STARTINGvector_size 20

namespace aisdi
{
template <typename Type>
class Vector
{


public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
    size_type rsrv_size;
    size_type vec_size;
    value_type *arr ;

    void reallocate() {
        rsrv_size<<=2;
        Type *tarr = new Type[rsrv_size];
        size_type i;
        for (i=0; i!=vec_size; i++)
        {
            tarr[i]=arr[i];
        }
        delete[] arr;
        arr = tarr;
    }

public:
    Vector()
    {
        vec_size=0;
        rsrv_size=STARTINGvector_size;
        arr = new value_type[rsrv_size];
    }


    ~Vector()
  {
      delete[] arr ;
  }


  Vector(std::initializer_list<Type> l)
  {
          vec_size = l.size();
          rsrv_size = vec_size;
          arr = new value_type[rsrv_size];
          auto element = l.begin();
          for( size_type i = 0; i < vec_size; i++ )
              arr[i] = *(element++);
   }
   
  Vector(const Vector& other) {
      vec_size = other.vec_size;
      rsrv_size = other.rsrv_size;
      arr = new value_type[rsrv_size];
      for (size_type i = 0; i < vec_size; i++)
          arr[i] = other.arr[i];
  }

   

  Vector(Vector&& other)
  {
      arr = other.arr;
      vec_size = other.vec_size;
      rsrv_size=other.rsrv_size;
      other.arr = nullptr;
      other.vec_size = 0;
      other.rsrv_size=0;
  }

  Vector& operator=(const Vector& other)
  { 
       if (this!=&other) {
           delete[] arr;
           vec_size = other.vec_size;
           rsrv_size = other.rsrv_size;
           arr = new Type[rsrv_size];
           size_type i;
           for (i = 0; i < vec_size; i++) {
               arr[i] = other.arr[i];
           }
       }
        return *this;
          
  }

  Vector& operator=(Vector&& other)
  {
      if(this != &other)
      {
          delete [] arr;
          arr= other.arr;
          vec_size = other.vec_size;
          rsrv_size =other.rsrv_size;
          other.arr = nullptr;
          other.vec_size = 0;
          other.rsrv_size =0;
      }
      return *this;
  }


  bool isEmpty() const
  {
    return (vec_size==0);
  }

  size_type getSize() const
  {
    return vec_size;
  }

  void append(const Type& item)
  {
      if (vec_size==rsrv_size)
      {
          reallocate();
      }
      arr[vec_size]=item;
      vec_size++;
  }

  void prepend(const Type& item)
  {

      if (vec_size==rsrv_size)
      {
          reallocate();
      }

      size_type i;

      for(i=vec_size;i!=0;--i)
      {
          arr[i]=arr[i-1];
      }
      arr[0]=item;
      vec_size++;

  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {

     if (insertPosition==cbegin()) {prepend(item); return;}
     if (insertPosition==cend()) {append(item); return; }

      size_type insert_index = insertPosition.givePointer()-arr;
      if (vec_size== rsrv_size) { reallocate(); }

        size_type i=vec_size;

        while (i!=insert_index)
        {
            arr[i]=arr[i-1];
            i--;
        }
        arr[i]=item;
        vec_size++;

  }

  Type popFirst()
  {
    if (isEmpty())  { throw std::out_of_range("empty class"); }

    value_type x=*begin();
      size_type i;
      for (i=0;i!=(vec_size-1);i++)
      {
          arr[i]=arr[i+1];
      }
      vec_size--;

    return x;
  }

  Type popLast()
  {

    if (isEmpty())  {throw std::out_of_range("empty class"); }
    value_type x;
    x=arr[vec_size-1];
    --vec_size;
      return x;
  }

  void erase(const const_iterator& possition)
  {

    if (isEmpty()){  throw std::out_of_range("You want to erase element from empty collection");}
    if (possition==end()) {throw std::out_of_range("You want to erase end of vector"); }


    const_iterator j=begin();
    size_type i=0;

    while (j!=possition)
    {
        i++; j++;
    }
    j++;

    while(j!=end())
    {
        arr[i]=arr[i+1];
        i++;
        j++;
    }
    vec_size--;

  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {

    if (&firstIncluded==&lastExcluded) { return;}
    const_iterator old=begin();
    size_type young=0;
    size_type counter=0;
    while (old!=firstIncluded)
    {
        young++; old++;
    }
    while (old!=lastExcluded)
    {
        old++;
        counter++;

    }
    while (old!=end())
    {
        arr[young]=arr[young+counter];
        old++;
        young++;
    }
    vec_size-=counter;

  }

  iterator begin()
  {
      iterator i = Iterator(arr, this);
      return i;
  }

  iterator end()
  {
      iterator i = Iterator(arr+vec_size, this);
      return i;
  }


  const_iterator cbegin() const
  {
      const_iterator i = ConstIterator(arr, this);
      return i;
  }

  const_iterator cend() const
  {
    const_iterator i= ConstIterator(arr+vec_size, this);
      return i;
  }


  const_iterator begin() const
  {
    return cbegin();
  }


    const_iterator end() const
  {
    return cend();
  }

};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;
  using this_pointer = const Vector<Type>*;


  explicit ConstIterator()
  {
      ptr= nullptr;
      this_vector= nullptr;
  }

  ConstIterator(pointer other, this_pointer other2)
  {
      ptr=other;
      this_vector=other2;
  }

  pointer givePointer() const
  {
      return ptr;
  }

  reference operator*() const
  {
      if(*this==this_vector->end()) {throw std::out_of_range("not allowed reference");}
      return *ptr;
  }

  value_type giveValue() const
  {
      return *ptr;
  }

  ConstIterator& operator++()
  {
        if(*this==this_vector->end()) {throw std::out_of_range("not allowed 1");}
        ptr++; return *this;

  }

  ConstIterator operator++(int)
  {
      if(*this==this_vector->end()) {throw std::out_of_range("not allowed 2");}
      ConstIterator i= *this;
      ptr++; return i;
  }

  ConstIterator& operator--()
  {
      if(*this==this_vector->begin()) {throw std::out_of_range("not allowed 3");}
       ptr--; return *this;
  }

  ConstIterator operator--(int)
  {
      if(*this==this_vector->begin()) {throw std::out_of_range("not allowed 4");}
      auto i=*this;
      ptr--; return i;

  }

  ConstIterator operator+(difference_type d) const
  {
        ConstIterator itt= *this;
        itt.ptr+=d; return itt;
  }

  ConstIterator operator-(difference_type d) const
  {
      ConstIterator itt = *this;
      itt.ptr-=d; return itt;
  }
    
  ConstIterator& operator=(const ConstIterator&other)
  {
      ptr=other.ptr;
      return *this;
  }

  bool operator==(const ConstIterator& other) const
  {
      return (ptr== other.ptr);
  }

  bool operator>=(const ConstIterator& other) const
  {
      return (ptr>=other.ptr);
  }
  bool operator<(const ConstIterator& other) const
  {
      return (ptr<other.ptr);
  }
  bool operator!=(const ConstIterator& other) const
  {
      return (ptr!= other.ptr);
  }

private:
    pointer ptr;
    this_pointer this_vector;

};

    template <typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
    {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;
        using this_pointer = Vector<Type>*;

        explicit Iterator()
        {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other)
        {}

        Iterator& operator++()
        {
            ConstIterator::operator++();
            return *this;
        }

        Iterator (pointer other, this_pointer other2)
                : ConstIterator(other, other2)
        {}

        Iterator operator++(int)
        {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--()
        {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int)
        {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const
        {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const
        {
            return ConstIterator::operator-(d);
        }

        reference operator*() const
        {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_VECTOR_H
