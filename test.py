import stocksnake as ss


b=ss.Position()
print(b.fen())

print("Stocksnake")
i_moves = b.legal_moves()
for _ in range(1):
    for mv in i_moves:
        b.push(mv)
        nxt = b.legal_moves()
        print(b.fen())
        print(b.stockfish_trace())
        print(b.stockfish_value())
        print()
        b.pop()

