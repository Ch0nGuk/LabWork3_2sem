#include "tests.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "DynamicArray.h"
#include "LinkedList.h"
#include "map.h"
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "ListSequence.h"
#include "sequence_factory.h"
#include "unzip.h"
#include "zip.h"

namespace
{
    // Завершает текущий тест ошибкой, если значения не совпали.
    template <typename T>
    void AssertEqual(const T& actual, const T& expected, const std::string& message)
    {
        if (!(actual == expected))
        {
            std::ostringstream error_stream;
            error_stream << message;
            throw std::runtime_error(error_stream.str());
        }
    }

    // Завершает текущий тест ошибкой, если условие ложно.
    void AssertTrue(bool condition, const std::string& message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }

    // Проверяет, что переданное действие выбрасывает исключение.
    template <typename Action>
    void AssertThrows(Action action, const std::string& message)
    {
        bool did_throw = false;

        try
        {
            action();
        }
        catch (const std::exception&)
        {
            did_throw = true;
        }

        if (!did_throw)
        {
            throw std::runtime_error(message);
        }
    }

    // Пример функции преобразования для тестов последовательностей.
    int Square(int value)
    {
        return value * value;
    }

    // Пример предиката для тестов фильтрации.
    bool IsEven(int value)
    {
        return value % 2 == 0;
    }

    // Пример сворачивающей функции для тестов Reduce.
    int Sum(int left, int right)
    {
        return left + right;
    }

    // Проверяет полное содержимое и длину целочисленной последовательности.
    void AssertSequenceContent(const Sequence<int>* sequence, const int* expected, int count, const std::string& message)
    {
        AssertEqual(sequence->GetLength(), count, message + ": unexpected length");
        for (int index = 0; index < count; index++)
        {
            AssertEqual(sequence->Get(index), expected[index], message + ": unexpected item");
        }
    }

    template <typename T>
    void AssertTypedSequenceContent(const Sequence<T>* sequence, const T* expected, int count, const std::string& message)
    {
        AssertEqual(sequence->GetLength(), count, message + ": unexpected length");
        for (int index = 0; index < count; index++)
        {
            AssertEqual(sequence->Get(index), expected[index], message + ": unexpected item");
        }
    }

    double Half(int value)
    {
        return value / 2.0;
    }

    bool IsPositive(int value)
    {
        return value > 0;
    }

    // Проверяет создание, изменение, Resize и ошибочные случаи DynamicArray.
    void TestDynamicArray()
    {
        int items[] = {1, 2, 3};

        DynamicArray<int> array(items, 3);
        AssertEqual(array.GetSize(), 3, "DynamicArray GetSize");
        AssertEqual(array.Get(0), 1, "DynamicArray Get first");
        AssertEqual(array.Get(2), 3, "DynamicArray Get last");

        array.Set(1, 5);
        AssertEqual(array.Get(1), 5, "DynamicArray Set");

        array.Resize(5);
        AssertEqual(array.GetSize(), 5, "DynamicArray Resize bigger");
        AssertEqual(array.Get(0), 1, "DynamicArray data preserved after grow");
        AssertEqual(array.Get(1), 5, "DynamicArray changed item preserved after grow");

        array.Resize(2);
        AssertEqual(array.GetSize(), 2, "DynamicArray Resize smaller");
        AssertEqual(array.Get(1), 5, "DynamicArray data preserved after shrink");

        AssertThrows([]() { DynamicArray<int> invalid(-1); }, "DynamicArray negative size must throw");
        AssertThrows([]() { DynamicArray<int> invalid(nullptr, 2); }, "DynamicArray null source must throw");
        AssertThrows([&array]() { array.Get(-1); }, "DynamicArray negative index must throw");
        AssertThrows([&array]() { array.Get(10); }, "DynamicArray too large index must throw");
        AssertThrows([&array]() { array.Set(10, 1); }, "DynamicArray Set out of range must throw");
        AssertThrows([&array]() { array.Resize(-5); }, "DynamicArray negative resize must throw");
    }

    // Проверяет базовые операции списка, подсписки, Concat и ошибочные случаи.
    void TestLinkedList()
    {
        int items[] = {1, 2, 3};

        LinkedList<int> list(items, 3);
        AssertEqual(list.GetSize(), 3, "LinkedList GetSize");
        AssertEqual(list.GetFirst(), 1, "LinkedList GetFirst");
        AssertEqual(list.GetLast(), 3, "LinkedList GetLast");

        list.Append(4);
        list.Prepend(0);
        list.InsertAt(2, 99);
        AssertEqual(list.GetSize(), 6, "LinkedList size after mutating operations");
        AssertEqual(list.Get(0), 0, "LinkedList Prepend");
        AssertEqual(list.Get(2), 99, "LinkedList InsertAt");
        AssertEqual(list.GetLast(), 4, "LinkedList Append");

        LinkedList<int> sub_list = list.GetSubList(1, 3);
        AssertEqual(sub_list.GetSize(), 3, "LinkedList GetSubList size");
        AssertEqual(sub_list.Get(0), 1, "LinkedList GetSubList first item");
        AssertEqual(sub_list.Get(2), 2, "LinkedList GetSubList last item");

        LinkedList<int> concat_list = list.Concat(sub_list);
        AssertEqual(concat_list.GetSize(), 9, "LinkedList Concat size");

        LinkedList<int> empty_list;
        AssertThrows([&empty_list]() { empty_list.GetFirst(); }, "LinkedList GetFirst on empty must throw");
        AssertThrows([&empty_list]() { empty_list.GetLast(); }, "LinkedList GetLast on empty must throw");
        AssertThrows([&list]() { list.Get(-1); }, "LinkedList negative Get must throw");
        AssertThrows([&list]() { list.Get(100); }, "LinkedList too large Get must throw");
        AssertThrows([&list]() { list.InsertAt(-1, 1); }, "LinkedList negative InsertAt must throw");
        AssertThrows([&list]() { list.InsertAt(100, 1); }, "LinkedList too large InsertAt must throw");
        AssertThrows([&list]() { list.GetSubList(3, 1); }, "LinkedList invalid GetSubList must throw");
        AssertThrows([]() { LinkedList<int> invalid(nullptr, 2); }, "LinkedList null source must throw");
    }

    // Проверяет mutable-семантику и операции sequence для массива.
    void TestMutableArraySequence()
    {
        int items[] = {1, 2, 3};
        int expected_after_insert[] = {0, 1, 99, 2, 3, 4};
        int expected_subsequence[] = {1, 99, 2};
        int expected_concat[] = {0, 1, 99, 2, 3, 4, 1, 99, 2};
        int expected_map[] = {0, 1, 9801, 4, 9, 16};
        int expected_where[] = {0, 2, 4};
        int expected_slice[] = {0, 7, 8, 4};

        Sequence<int>* sequence = new MutableArraySequence<int>(items, 3);

        Sequence<int>* appended = sequence->Append(4);
        AssertTrue(appended == sequence, "MutableArraySequence Append must return same object");
        sequence = appended;

        Sequence<int>* prepended = sequence->Prepend(0);
        AssertTrue(prepended == sequence, "MutableArraySequence Prepend must return same object");
        sequence = prepended;

        Sequence<int>* inserted = sequence->InsertAt(2, 99);
        AssertTrue(inserted == sequence, "MutableArraySequence InsertAt must return same object");
        sequence = inserted;
        AssertSequenceContent(sequence, expected_after_insert, 6, "MutableArraySequence content after insert");

        Sequence<int>* subsequence = sequence->GetSubsequence(1, 3);
        AssertSequenceContent(subsequence, expected_subsequence, 3, "MutableArraySequence GetSubsequence");

        Sequence<int>* concatenated = sequence->Concat(*subsequence);
        AssertSequenceContent(concatenated, expected_concat, 9, "MutableArraySequence Concat");

        ListSequenceFactory<int> factory;
        Sequence<int>* mapped = Map<int, int>(*sequence, Square, factory);
        AssertSequenceContent(mapped, expected_map, 6, "MutableArraySequence Map");

        Sequence<int>* filtered = sequence->Where(IsEven);
        AssertSequenceContent(filtered, expected_where, 3, "MutableArraySequence Where");

        AssertEqual(sequence->Reduce(Sum, 0), 109, "MutableArraySequence Reduce");

        int replacement_items[] = {7, 8};
        Sequence<int>* replacement = new MutableArraySequence<int>(replacement_items, 2);
        Sequence<int>* sliced = sequence->Slice(1, 4, *replacement);
        AssertSequenceContent(sliced, expected_slice, 4, "MutableArraySequence Slice");

        AssertThrows([sequence]() { sequence->GetSubsequence(-1, 1); }, "MutableArraySequence invalid subsequence must throw");
        AssertThrows([sequence]() { sequence->InsertAt(100, 1); }, "MutableArraySequence invalid insert must throw");
        AssertThrows([sequence, replacement]() { sequence->Slice(10, 1, *replacement); }, "MutableArraySequence invalid slice must throw");

        delete sequence;
        delete subsequence;
        delete concatenated;
        delete mapped;
        delete filtered;
        delete replacement;
        delete sliced;
    }

    // Проверяет immutable-семантику и операции sequence для массива.
    void TestImmutableArraySequence()
    {
        int items[] = {1, 2, 3};
        int expected_original[] = {1, 2, 3};
        int expected_append[] = {1, 2, 3, 4};
        int expected_prepend[] = {0, 1, 2, 3};
        int expected_insert[] = {1, 99, 2, 3};
        int expected_map[] = {1, 4, 9};
        int expected_where[] = {2};
        int expected_slice[] = {1, 7, 8, 3};

        Sequence<int>* sequence = new ImmutableArraySequence<int>(items, 3);
        Sequence<int>* appended = sequence->Append(4);
        Sequence<int>* prepended = sequence->Prepend(0);
        Sequence<int>* inserted = sequence->InsertAt(1, 99);

        AssertTrue(appended != sequence, "ImmutableArraySequence Append must return new object");
        AssertTrue(prepended != sequence, "ImmutableArraySequence Prepend must return new object");
        AssertTrue(inserted != sequence, "ImmutableArraySequence InsertAt must return new object");

        AssertSequenceContent(sequence, expected_original, 3, "ImmutableArraySequence original content preserved");
        AssertSequenceContent(appended, expected_append, 4, "ImmutableArraySequence Append result");
        AssertSequenceContent(prepended, expected_prepend, 4, "ImmutableArraySequence Prepend result");
        AssertSequenceContent(inserted, expected_insert, 4, "ImmutableArraySequence InsertAt result");

        ListSequenceFactory<int> factory;
        Sequence<int>* mapped = Map<int, int>(*sequence, Square, factory);
        Sequence<int>* filtered = sequence->Where(IsEven);
        AssertSequenceContent(mapped, expected_map, 3, "ImmutableArraySequence Map");
        AssertSequenceContent(filtered, expected_where, 1, "ImmutableArraySequence Where");
        AssertEqual(sequence->Reduce(Sum, 0), 6, "ImmutableArraySequence Reduce");

        int replacement_items[] = {7, 8};
        Sequence<int>* replacement = new ImmutableArraySequence<int>(replacement_items, 2);
        Sequence<int>* sliced = sequence->Slice(1, 1, *replacement);
        AssertSequenceContent(sliced, expected_slice, 4, "ImmutableArraySequence Slice");

        AssertThrows([sequence]() { sequence->GetSubsequence(5, 10); }, "ImmutableArraySequence invalid subsequence must throw");
        AssertThrows([sequence]() { sequence->InsertAt(-1, 1); }, "ImmutableArraySequence invalid insert must throw");
        AssertThrows([sequence, replacement]() { sequence->Slice(0, 10, *replacement); }, "ImmutableArraySequence invalid slice must throw");

        delete sequence;
        delete appended;
        delete prepended;
        delete inserted;
        delete mapped;
        delete filtered;
        delete replacement;
        delete sliced;
    }

    // Проверяет поведение list-based sequence и основные высокоуровневые операции.
    void TestListSequence()
    {
        int items[] = {1, 2, 3};
        int expected_after_insert[] = {0, 1, 99, 2, 3, 4};
        int expected_subsequence[] = {1, 99, 2};
        int expected_map[] = {0, 1, 9801, 4, 9, 16};
        int expected_where[] = {0, 2, 4};
        int expected_slice[] = {0, 7, 8, 4};

        Sequence<int>* sequence = new ListSequence<int>(items, 3);
        sequence = sequence->Append(4);
        sequence = sequence->Prepend(0);
        sequence = sequence->InsertAt(2, 99);
        AssertSequenceContent(sequence, expected_after_insert, 6, "ListSequence content after insert");

        Sequence<int>* subsequence = sequence->GetSubsequence(1, 3);
        AssertSequenceContent(subsequence, expected_subsequence, 3, "ListSequence GetSubsequence");

        ListSequenceFactory<int> factory;
        Sequence<int>* mapped = Map<int, int>(*sequence, Square, factory);
        Sequence<int>* filtered = sequence->Where(IsEven);
        AssertSequenceContent(mapped, expected_map, 6, "ListSequence Map");
        AssertSequenceContent(filtered, expected_where, 3, "ListSequence Where");
        AssertEqual(sequence->Reduce(Sum, 0), 109, "ListSequence Reduce");

        int replacement_items[] = {7, 8};
        Sequence<int>* replacement = new ListSequence<int>(replacement_items, 2);
        Sequence<int>* sliced = sequence->Slice(1, 4, *replacement);
        AssertSequenceContent(sliced, expected_slice, 4, "ListSequence Slice");

        AssertThrows([sequence]() { sequence->GetSubsequence(-1, 1); }, "ListSequence invalid subsequence must throw");
        AssertThrows([sequence]() { sequence->InsertAt(100, 1); }, "ListSequence invalid insert must throw");
        AssertThrows([sequence, replacement]() { sequence->Slice(10, 1, *replacement); }, "ListSequence invalid slice must throw");

        delete sequence;
        delete subsequence;
        delete mapped;
        delete filtered;
        delete replacement;
        delete sliced;
    }

    // Проверяет порядок обхода enumerator и поведение на пустой последовательности.
    void TestEnumerators()
    {
        int items[] = {1, 2, 3};

        Sequence<int>* array_sequence = new MutableArraySequence<int>(items, 3);
        IEnumerator<int>* array_enumerator = array_sequence->GetEnumerator();
        int expected_index = 0;

        while (array_enumerator->MoveNext())
        {
            AssertEqual(array_enumerator->Current(), items[expected_index], "Array enumerator order");
            expected_index++;
        }
        AssertEqual(expected_index, 3, "Array enumerator count");

        Sequence<int>* list_sequence = new ListSequence<int>(items, 3);
        IEnumerator<int>* list_enumerator = list_sequence->GetEnumerator();
        expected_index = 0;

        while (list_enumerator->MoveNext())
        {
            AssertEqual(list_enumerator->Current(), items[expected_index], "List enumerator order");
            expected_index++;
        }
        AssertEqual(expected_index, 3, "List enumerator count");

        Sequence<int>* empty_sequence = new ListSequence<int>();
        IEnumerator<int>* empty_enumerator = empty_sequence->GetEnumerator();
        AssertTrue(!empty_enumerator->MoveNext(), "Empty enumerator must stop immediately");

        delete array_enumerator;
        delete array_sequence;
        delete list_enumerator;
        delete list_sequence;
        delete empty_enumerator;
        delete empty_sequence;
    }

    void TestMapZipUnzip()
    {
        int items[] = {1, 2, 3, 4};
        double expected_half[] = {0.5, 1.0, 1.5, 2.0};
        bool expected_positive[] = {true, true, true, true};
        std::pair<int, int> expected_zip[] = {
            std::make_pair(1, 10),
            std::make_pair(2, 20),
            std::make_pair(3, 30)
        };
        int expected_first_unzip[] = {1, 2, 3};
        int expected_second_unzip[] = {10, 20, 30};

        Sequence<int>* mutable_sequence = new MutableArraySequence<int>(items, 4);
        Sequence<int>* immutable_sequence = new ImmutableArraySequence<int>(items, 4);
        Sequence<int>* list_sequence = new ListSequence<int>(items, 4);
        int other_items[] = {10, 20, 30};
        Sequence<int>* other_sequence = new ListSequence<int>(other_items, 3);
        Sequence<int>* empty_sequence = new ListSequence<int>();

        MutableArraySequenceFactory<double> mutable_double_factory;
        ImmutableArraySequenceFactory<double> immutable_double_factory;
        ListSequenceFactory<bool> bool_factory;
        ListSequenceFactory<std::pair<int, int>> pair_factory;
        ListSequenceFactory<int> int_factory;

        Sequence<double>* mutable_mapped = Map<int, double>(*mutable_sequence, Half, mutable_double_factory);
        Sequence<double>* immutable_mapped = Map<int, double>(*immutable_sequence, Half, immutable_double_factory);
        Sequence<bool>* bool_mapped = Map<int, bool>(*list_sequence, IsPositive, bool_factory);
        Sequence<double>* empty_mapped = Map<int, double>(*empty_sequence, Half, immutable_double_factory);

        AssertTypedSequenceContent(mutable_mapped, expected_half, 4, "Generic Map mutable source to double");
        AssertTypedSequenceContent(immutable_mapped, expected_half, 4, "Generic Map immutable source to double");
        AssertTypedSequenceContent(bool_mapped, expected_positive, 4, "Generic Map list source to bool");
        AssertEqual(empty_mapped->GetLength(), 0, "Generic Map empty sequence");

        Sequence<std::pair<int, int>>* zipped = Zip<int, int>(*mutable_sequence, *other_sequence, pair_factory);
        Sequence<std::pair<int, int>>* zipped_empty = Zip<int, int>(*empty_sequence, *other_sequence, pair_factory);

        AssertTypedSequenceContent(zipped, expected_zip, 3, "Zip must pair elements up to shorter sequence");
        AssertEqual(zipped_empty->GetLength(), 0, "Zip with empty sequence must be empty");

        std::pair<Sequence<int>*, Sequence<int>*> unzipped = Unzip<int, int>(*zipped, int_factory, int_factory);
        AssertSequenceContent(unzipped.first, expected_first_unzip, 3, "Unzip first result");
        AssertSequenceContent(unzipped.second, expected_second_unzip, 3, "Unzip second result");

        std::pair<Sequence<int>*, Sequence<int>*> empty_unzipped = Unzip<int, int>(*zipped_empty, int_factory, int_factory);
        AssertEqual(empty_unzipped.first->GetLength(), 0, "Unzip first empty result");
        AssertEqual(empty_unzipped.second->GetLength(), 0, "Unzip second empty result");

        delete mutable_sequence;
        delete immutable_sequence;
        delete list_sequence;
        delete other_sequence;
        delete empty_sequence;
        delete mutable_mapped;
        delete immutable_mapped;
        delete bool_mapped;
        delete empty_mapped;
        delete zipped;
        delete zipped_empty;
        delete unzipped.first;
        delete unzipped.second;
        delete empty_unzipped.first;
        delete empty_unzipped.second;
    }
}

// Запускает полный локальный набор тестов ядра лабы.
void RunAllTests()
{
    TestDynamicArray();
    TestLinkedList();
    TestMutableArraySequence();
    TestImmutableArraySequence();
    TestListSequence();
    TestEnumerators();
    TestMapZipUnzip();

    std::cout << "All tests passed." << std::endl;
}
