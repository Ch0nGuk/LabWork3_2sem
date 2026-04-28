#ifndef SEQUENCE_UI_UTILS_H
#define SEQUENCE_UI_UTILS_H

#include <string>
#include <utility>

#include "ui_state.h"

bool ReadInt(const std::string& prompt, int& value);
void PrintSequence(const Sequence<int>* sequence);
void PrintSequence(const Sequence<double>* sequence);
void PrintSequence(const Sequence<std::pair<int, int>>* sequence);
void PrintStoredSequences(const UiState& state);
void PrintStoredDoubleSequences(const UiState& state);
void PrintStoredPairSequences(const UiState& state);
bool SelectSequenceIndex(const UiState& state, const std::string& prompt, int& index);
bool SelectDoubleSequenceIndex(const UiState& state, const std::string& prompt, int& index);
bool SelectPairSequenceIndex(const UiState& state, const std::string& prompt, int& index);

#endif // SEQUENCE_UI_UTILS_H
