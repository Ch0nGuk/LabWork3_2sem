#ifndef ZIP_H
#define ZIP_H

#include <memory>
#include <utility>

#include "IEnumerator.h"
#include "Sequence.h"
#include "sequence_factory.h"

template <typename T1, typename T2>
Sequence<std::pair<T1, T2>>* Zip(
    const Sequence<T1>& first,
    const Sequence<T2>& second,
    const SequenceFactory<std::pair<T1, T2>>& factory)
{
    std::unique_ptr<Sequence<std::pair<T1, T2>>> result(factory.Create());
    std::unique_ptr<IEnumerator<T1>> first_enumerator(first.GetEnumerator());
    std::unique_ptr<IEnumerator<T2>> second_enumerator(second.GetEnumerator());

    while (first_enumerator->MoveNext() && second_enumerator->MoveNext())
    {
        Sequence<std::pair<T1, T2>>* next =
            result->Append(std::make_pair(first_enumerator->Current(), second_enumerator->Current()));

        if (next != result.get())
        {
            result.reset(next);
        }
    }

    return result.release();
}

#endif // ZIP_H
