#ifndef UNZIP_H
#define UNZIP_H

#include <utility>

#include "../UniquePtr.h"
#include "IEnumerator.h"
#include "Sequence.h"
#include "sequence_factory.h"

template <typename T1, typename T2>
std::pair<Sequence<T1>*, Sequence<T2>*> Unzip(
    const Sequence<std::pair<T1, T2>>& source,
    const SequenceFactory<T1>& first_factory,
    const SequenceFactory<T2>& second_factory)
{
    UniquePtr<Sequence<T1>> first_result(first_factory.Create());
    UniquePtr<Sequence<T2>> second_result(second_factory.Create());
    UniquePtr<IEnumerator<std::pair<T1, T2>>> enumerator(source.GetEnumerator());

    while (enumerator->MoveNext())
    {
        const std::pair<T1, T2>& value = enumerator->Current();

        Sequence<T1>* next_first = first_result->Append(value.first);
        if (next_first != first_result.get())
        {
            first_result.reset(next_first);
        }

        Sequence<T2>* next_second = second_result->Append(value.second);
        if (next_second != second_result.get())
        {
            second_result.reset(next_second);
        }
    }

    return std::make_pair(first_result.release(), second_result.release());
}

#endif // UNZIP_H
