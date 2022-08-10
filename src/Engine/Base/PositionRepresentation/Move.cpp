#include "Move.hpp"


Move::Move() = default;
Move::Move(uint8_t from, uint8_t to, uint8_t attacker_type, uint8_t attacker_side, uint8_t defender_type, uint8_t defender_side, uint8_t flag) {
    this->_from = from;
    this->_to = to;

    this->_attacker_type = attacker_type;
    this->_attacker_side = attacker_side;

    this->_defender_type = defender_type;
    this->_defender_side = defender_side;

    this->_flag = flag;
}
bool operator ==(Move left, Move right) {
    return (left._from == right._from and left._to == right._to and left._attacker_type == right._attacker_type and left._attacker_side == right._attacker_side and left._defender_type == right._defender_type and left._defender_side == right._defender_side and left._flag == right._flag);
}