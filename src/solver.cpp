#include "solver.h"
#include <queue>
#include <map>
#include <set>
#include <algorithm>

void update(std::vector<Board> &next, const Board &current, std::pair<unsigned, unsigned> next_ind)
{
    auto cur_ind = current.empty_pos();
    auto next_board = current.swap(cur_ind, next_ind);
    next.push_back(next_board);
}

std::vector<Board> generate_next(const Board &current)
{
    std::vector<Board> next;
    auto empty_pos = current.empty_pos();
    if (empty_pos.first > 0)
        update(next, current, {empty_pos.first - 1, empty_pos.second});
    if (empty_pos.first + 1 < current.size())
        update(next, current, {empty_pos.first + 1, empty_pos.second});
    if (empty_pos.second > 0)
        update(next, current, {empty_pos.first, empty_pos.second - 1});
    if (empty_pos.second + 1 < current.size())
        update(next, current, {empty_pos.first, empty_pos.second + 1});
    return next;
}

Solver::Solver(const Board &board)
{
    if (!board.is_solvable())
        return;

    auto cmp1 = [](const Board &a, const Board &b)
    {
        for (unsigned i = 0; i < a.size(); i++) {
            for (unsigned j = 0; j < a.size(); j++)
            {
                if (a[i][j] != b[i][j])
                {
                    return a[i][j] < b[i][j];
                }
            }
        }
        return false;
    };

    auto cmp2 = [](const Board &left, const Board &right)
    {
        return left.hamming() + left.manhattan() > right.hamming() + right.manhattan();
    };

    std::priority_queue<Board, std::vector<Board>, decltype(cmp2)> queue(cmp2);
    std::map<Board, Board, decltype(cmp1)> parent(cmp1);
    queue.push(board);

    std::map<Board, unsigned, decltype(cmp1)> scores(cmp1);
    scores.emplace(board, 0);
    std::set<Board, decltype(cmp1)> used(cmp1);

    while (!queue.empty()) {
        auto current = queue.top();
        if (current.is_goal())
            break;
        queue.pop();
        used.insert(current);
        auto next = generate_next(current);
        for (const auto &v : next) {
            unsigned score = scores[current] + v.hamming() + v.manhattan();
            if (used.count(v) && score >= scores[v])
                continue;
            parent[v] = current;
            scores[v] = score;
            if (!used.count(v))
                queue.push(v);
        }
    }

    m_moves.push_back(queue.top());
    while (m_moves.back() != board)
    {
        m_moves.push_back(parent[m_moves.back()]);
    }
    std::reverse(m_moves.begin(), m_moves.end());
}

std::size_t Solver::moves() const
{
    if (m_moves.empty())
        return 0;
    return m_moves.size() - 1;
}