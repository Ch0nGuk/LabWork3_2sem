#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <memory>

#include "IEnumerator.h"

template <typename T>
class Sequence
{
public:
    virtual ~Sequence() = default;

    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual const T& Get(int index) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(int index, const T& item) = 0;
    virtual Sequence<T>* GetSubsequence(int start_index, int end_index) const = 0;
    virtual Sequence<T>* Concat(const Sequence<T>& other) const = 0;
    virtual Sequence<T>* Slice(int start_index, int count) const = 0;
    virtual Sequence<T>* Slice(int start_index, int count, const Sequence<T>& replacement) const = 0;
    virtual IEnumerator<T>* GetEnumerator() const = 0;

    virtual Sequence<T>* Instance() = 0;

    virtual Sequence<T>* Where(bool (*predicate)(T)) const = 0;
    T Reduce(T (*func)(T, T), T start_value) const
    {
        T result = start_value;
        std::unique_ptr<IEnumerator<T>> enumerator(GetEnumerator());

        while (enumerator->MoveNext())
        {
            result = func(result, enumerator->Current());
        }

        return result;
    }

    
protected:
    virtual Sequence<T>* AppendInternal(const T& item) = 0;
    virtual Sequence<T>* PrependInternal(const T& item) = 0;
    virtual Sequence<T>* InsertAtInternal(int index, const T& item) = 0;
};

#endif // SEQUENCE_H
