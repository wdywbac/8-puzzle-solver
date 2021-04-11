#include <random>
#include <algorithm>
#include "board.h"

Board Board::create_goal(const unsigned size)
{
    std::vector<std::vector<unsigned>> data(size, std::vector<unsigned> (size) );
    unsigned cnt = 1;
    for (unsigned i = 0; i < size; i++) {
        for (unsigned j = 0; j < size; j++) {
            data[i][j] = cnt;
            cnt++;
            cnt %= size * size;
        }
    }
    Board b(data);
    return b;
}

Board::Board(const unsigned size)
{
    std::vector<unsigned> shuffled(size * size);
    for (unsigned i = 0; i < shuffled.size(); i++)
    {
        shuffled[i] = i;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), g);
    table_.resize(size, std::vector<unsigned>(size));
    for (unsigned i = 0; i < size; i++)
    {
        for (unsigned j = 0; j < size; j++)
        {
            table_[i][j] = shuffled[i * size + j];
            if (table_[i][j] == 0)
                empty_pos_ = std::make_pair(i, j);
        }
    }
}

Board::Board(const std::vector<std::vector<unsigned>> & data): table_(data)
{
    for (unsigned i = 0; i < data.size(); i++)
    {
        for (unsigned j = 0; j < data.size(); j++)
        {
            if (data[i][j] == 0)
            {
                empty_pos_ = std::make_pair(i, j);
                break;
            }
        }
    }
}

std::size_t Board::size() const
{
    return table_.size();
}

bool Board::is_goal() const
{
    return hamming() == 0;
}

unsigned Board::hamming() const
{
    if (size() == 0)
        return 0;
    unsigned cnt = 0;
    unsigned exp = 0;
    unsigned amount = size() * size() - 1;
    for (unsigned i = 0; i < size(); i++) {
        for (unsigned j = 0; j < size(); j++) {
            if (exp < amount) {
                exp++;
            } else {
                exp = 0;
            }
            if (table_[i][j] != exp) {
                cnt++;
            }
        }
    }
    return cnt;
}

unsigned Board::manhattan() const
{
    unsigned sum = 0;
    unsigned exp = 0;
    unsigned amount = size() * size() - 1;
    for (unsigned i = 0; i < size(); i++) {
        for (unsigned j = 0; j < size(); j++) {
            if (exp < amount) {
                exp++;
            } else {
                exp = 0;
            }
            if (table_[i][j] != exp && table_[i][j] != 0) {
                sum += manhattan(table_[i][j], i, j);
            }
        }
    }
    return sum;
}

unsigned Board::manhattan(unsigned curr, unsigned i, unsigned j) const
{
    unsigned row = abs((int)((curr - 1) / size() - i));
    unsigned col = abs((int)((curr - 1) % size() - j));
    return row + col;
}

std::string Board::to_string() const
{
    std::string result;
    for (const auto& row : table_)
    {
        for (auto item : row)
        {
            result += item ? std::to_string(item) + " " : "_ ";
        }
        result += "\n";
    }
    return result;
}

std::pair<unsigned, unsigned> Board::empty_pos() const
{
    return empty_pos_;
}

Board Board::swap(std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b) const
{
    auto result = table_;
    std::swap(result[a.first][a.second], result[b.first][b.second]);
    return Board(result);;
}

bool Board::is_solvable() const
{
    if (is_goal())
        return true;
    unsigned cnt = 0;
    for (unsigned i = 0; i < size(); i++)
    {
        for (unsigned j = 0; j < size(); j++)
        {
            for (unsigned x = i; x < size(); x++)
            {
                for (unsigned y = (i == x) ? j + 1 : 0; y < size(); y++)
                {
                    cnt += (table_[i][j] && table_[x][y]) ? table_[x][y] < table_[i][j] : 0;
                }
            }
        }
    }
    if (size() % 2 == 0)
    {
        cnt += 1 + (empty_pos_.first * size() + empty_pos_.second) / size();
    }
    return cnt % 2 == 0;
}

const std::vector<unsigned> & Board::operator [] (const std::size_t i) const
{
    return table_[i];
}

bool operator == (const Board & lhs, const Board & rhs)
{
    return lhs.table_ == rhs.table_;
}

bool operator != (const Board & lhs, const Board & rhs)
{
    return !(lhs.table_ == rhs.table_);
}