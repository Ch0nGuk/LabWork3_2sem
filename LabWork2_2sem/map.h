#ifndef MAP_H
#define MAP_H

#include "../UniquePtr.h"
#include "IEnumerator.h"
#include "Sequence.h"
#include "sequence_factory.h"

template <typename T, typename T2, typename Mapper>
Sequence<T2>* Map(const Sequence<T>& source, Mapper mapper, const SequenceFactory<T2>& factory)
{
    UniquePtr<Sequence<T2>> result(factory.Create());
    UniquePtr<IEnumerator<T>> enumerator(source.GetEnumerator());

    while (enumerator->MoveNext())
    {
        Sequence<T2>* next = result->Append(mapper(enumerator->Current()));
        if (next != result.get())
        {
            result.reset(next);
        }
    }

    return result.release();
}

#endif // MAP_H
