#include "RepetitionHistory.hpp"


RepetitionHistory::RepetitionHistory() = default;
void RepetitionHistory::add_position(ZobristHash hash) {
    this->_hashes.push_back(hash);
}
void RepetitionHistory::clear() {
    this->_hashes.clear();
}
uint8_t RepetitionHistory::get_repetition_number(ZobristHash hash) {
    uint8_t ctr = 0;

    for (auto hash1 : this->_hashes) if (hash == hash1) ctr = ctr + 1;

    return ctr;
}