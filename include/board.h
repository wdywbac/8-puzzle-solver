#pragma once

#include <string>
#include <vector>

class Board
{
public:
    static Board create_goal(unsigned size);

    Board() = default;

    Board(const Board & other) = default;

    Board & operator = (const Board & other) = default;

    explicit Board(unsigned size);

    explicit Board(const std::vector<std::vector<unsigned>> & data);

    std::size_t size() const;

    bool is_goal() const;

    unsigned hamming() const;

    unsigned manhattan() const;

    unsigned manhattan(unsigned goal, unsigned i, unsigned j) const;

    std::string to_string() const;

    bool is_solvable() const;

    std::pair<unsigned , unsigned > empty_pos() const;

    friend bool operator == (const Board & /*lhs*/, const Board & /*rhs*/);

    friend bool operator != (const Board & /*lhs*/, const Board & /*rhs*/);

    const std::vector<unsigned> & operator [] (std::size_t i) const;

    friend std::ostream & operator << (std::ostream & out, const Board & board)
    { return out << board.to_string(); }

    Board swap(std::pair<unsigned, unsigned>, std::pair<unsigned, unsigned>) const;

private:
    std::vector<std::vector<unsigned>> table_;
    std::pair<unsigned, unsigned> empty_pos_;
};
