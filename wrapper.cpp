#include <pybind11/pybind11.h>
#include <iostream>
#include "thread.h"
#include "position.h"
#include "uci.h"
#include "evaluate.h"


int add(int i, int j) {
    return i + +j;
}
namespace py = pybind11;
const char* StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";



namespace PSQT {
    void init();
}

void init_stockfish() {
    UCI::init(Options);
    PSQT::init();
    Bitboards::init();
    Position::init();
    Bitbases::init();
    Search::init();
    Pawns::init();
    Threads.set(Options["Threads"]);
    Search::clear(); // After threads are up
}



class EasyPosition {
public:
    EasyPosition(const std::string& fen) : thread_(Threads.main()) {
        states_ = StateListPtr(new std::deque<StateInfo>(1)); // Drop old and create a new one
        position_.set(fen, false, &states_->back(), thread_.get());

    }
    EasyPosition() : EasyPosition(StartFEN){}

    std::string fen() {
        return position_.fen();
    }

    void make_move(int16_t mv) {
        states_->emplace_back();
        Move move = static_cast<Move>(mv);
        position_.do_move(move, states_->back());
        move_seq_.push_back(move);
    }

    int16_t undo_last_move() {
        int16_t last_move = move_seq_.back();
        position_.undo_move(move_seq_.back());
        move_seq_.pop_back();
        states_->pop_back();
        return last_move;
    }

    py::list legal_moves() {
        py::list moves;
        for (const ExtMove& move : MoveList<LEGAL>(position_)) {
            moves.append(static_cast<int16_t>(move.move));
        }
        return moves;
    }

    double stockfish_value() {
        return double(Eval::evaluate(position_)) / PawnValueEg;
    }

    std::string stockfish_trace() {
        return Eval::trace(position_);
    }

    py::list evasion_moves() {
        py::list moves;
        for (const ExtMove& move : MoveList<EVASIONS>(position_)) {
            moves.append(static_cast<int16_t>(move.move));
        }
        return moves;
    }

    py::list capture_moves() {
        py::list moves;
        for (const ExtMove& move : MoveList<CAPTURES>(position_)) {
            moves.append(static_cast<int16_t>(move.move));
        }
        return moves;
    }

private:
    std::deque<Move> move_seq_;
    std::unique_ptr<Thread> thread_;
    StateListPtr states_;
    Position position_;
};


PYBIND11_MODULE(_stocksnake, m) {
    m.doc() = R"pbdoc(
        Stockfish wrapper.
    )pbdoc";

    py::class_<EasyPosition>(m, "Position")
            .def(py::init<>())
            .def("fen", &EasyPosition::fen)
            .def("legal_moves", &EasyPosition::legal_moves)
            .def("evasion_moves", &EasyPosition::evasion_moves)
            .def("capture_moves", &EasyPosition::capture_moves)
            .def("push", &EasyPosition::make_move)
            .def("pop", &EasyPosition::undo_last_move)
            .def("stockfish_value", &EasyPosition::stockfish_value)
            .def("stockfish_trace", &EasyPosition::stockfish_trace)

            ;



    m.def("_init", &init_stockfish, R"pbdoc(Init
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}