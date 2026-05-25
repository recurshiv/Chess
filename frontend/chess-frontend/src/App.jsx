import { useState, useEffect } from 'react';
import './App.css';

const API_URL = 'http://localhost:9090'; 

function App() {
  const [board, setBoard] = useState(Array(8).fill(Array(8).fill(0)));
  const [turn, setTurn] = useState('white');
  const [selectedSquare, setSelectedSquare] = useState(null);
  const [legalMoves, setLegalMoves] = useState(Array(8).fill(Array(8).fill(false)));
  
  // Game state modifiers
  const [moveHistory, setMoveHistory] = useState([]);
  const [pendingPromotion, setPendingPromotion] = useState(null);
  const [gameState, setGameState] = useState(0); // 0: Active, 1: White Win, 2: Black Win, 3: Stalemate, 4: Draw

  const fetchBoard = async () => {
    try {
      const res = await fetch(`${API_URL}/board`);
      const data = await res.json();
      
      const translatedBoard = data.board.map(row => 
        Array.from(row).map(cell => {
          if (cell === 0 || cell === null || cell === '\0') return 0;
          if (typeof cell === 'number') return String.fromCharCode(cell);
          return cell;
        })
      );

      setBoard(translatedBoard);
      setTurn(data.turn);
      
      // Map state integer from C++ backend payload
      if (data.state !== undefined) {
        setGameState(data.state);
      }

      setSelectedSquare(null);
      setLegalMoves(Array(8).fill(Array(8).fill(false)));
    } catch (err) {
      console.error("Failed to connect to backend", err);
    }
  };

  useEffect(() => {
    fetchBoard();
  }, []);

  const handleRestart = async () => {
    try {
      // 1. Tell C++ to reset its brain
      await fetch(`${API_URL}/reset`, { method: 'POST' });
      
      // 2. Clear React's memory
      setMoveHistory([]);
      setGameState(0);
      
      // 3. Fetch the fresh board from C++
      fetchBoard();
    } catch (err) {
      console.error("Failed to reset game", err);
    }
  };

  // --- HELPERS ---

  const playSound = (type) => {
    const rootStyles = getComputedStyle(document.documentElement);
    let soundUrl = rootStyles.getPropertyValue(`--theme-sound-set-mp3-${type}`).trim();
    if (soundUrl.startsWith("url(")) {
      soundUrl = soundUrl.replace(/^url\(['"]?/, '').replace(/['"]?\)$/, '');
      const audio = new Audio(soundUrl);
      audio.play().catch(e => console.log("Audio playback blocked:", e));
    }
  };

  const getPieceStyle = (char) => {
    if (!char || char === 0) return {};
    const strChar = String(char);
    const color = strChar === strChar.toUpperCase() ? 'w' : 'b';
    const piece = strChar.toLowerCase();
    return { backgroundImage: `var(--theme-piece-set-${color}${piece})` };
  };

  const getSquareName = (x, y) => {
    const files = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'];
    return `${files[y]}${8 - x}`;
  };

  const getMoveNotation = (fromX, fromY, toX, toY, pieceChar, isCapture, promo) => {
    const isPawn = String(pieceChar).toLowerCase() === 'p';
    const dest = getSquareName(toX, toY);
    
    let notation = '';
    if (isPawn) {
      if (isCapture) {
        const fromFile = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'][fromY];
        notation = `${fromFile}x${dest}`; 
      } else {
        notation = dest; 
      }
    } else {
      notation = `${String(pieceChar).toUpperCase()}${isCapture ? 'x' : ''}${dest}`;
    }
    
    if (promo) notation += `=${String(promo).toUpperCase()}`; 
    return notation;
  };

  // --- GAME INTERACTION LOGIC ---

  const fetchMovesForPiece = async (x, y) => {
    if (gameState !== 0) return; // Block interaction if game is over
    setSelectedSquare({ x, y });
    try {
      const res = await fetch(`${API_URL}/moves/${x}/${y}`);
      const data = await res.json();
      setLegalMoves(data.moves);
    } catch (err) {
      console.error("Failed to fetch moves", err);
    }
  };

  const executeMove = async (fromX, fromY, toX, toY, isCapture, promotionChoice = null) => {
    const piece = board[fromX][fromY];
    const notation = getMoveNotation(fromX, fromY, toX, toY, piece, isCapture, promotionChoice);
    
    setMoveHistory(prev => [...prev, notation]);

    try {
      await fetch(`${API_URL}/move`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ 
          from_x: fromX, from_y: fromY, 
          to_x: toX, to_y: toY,
          promotion: promotionChoice 
        })
      });
      
      playSound(isCapture ? 'capture' : 'move-self');
      
      const res = await fetch(`${API_URL}/board?t=${Date.now()}`);
      const data = await res.json();
      
      const translatedBoard = data.board.map(row => 
        Array.from(row).map(cell => {
          if (cell === 0 || cell === null || cell === '\0') return 0;
          if (typeof cell === 'number') return String.fromCharCode(cell);
          return cell;
        })
      );

      setBoard(translatedBoard);
      setTurn(data.turn);
      if (data.state !== undefined) setGameState(data.state);
      
      setSelectedSquare(null);
      setLegalMoves(Array(8).fill(Array(8).fill(false)));
      
    } catch (err) {
      console.error("Failed to execute move", err);
    }
  };

  const handleMoveIntent = (destX, destY) => {
    if (!selectedSquare || gameState !== 0) return;
    
    if (legalMoves[destX][destY]) {
      const fromX = selectedSquare.x;
      const fromY = selectedSquare.y;
      const piece = board[fromX][fromY];
      const isCapture = board[destX][destY] !== 0;

      if ((piece === 'P' && destX === 0) || (piece === 'p' && destX === 7)) {
        setPendingPromotion({ 
          fromX, fromY, destX, destY, isCapture, 
          color: piece === 'P' ? 'w' : 'b' 
        });
        return;
      }

      executeMove(fromX, fromY, destX, destY, isCapture);
    } else {
      setSelectedSquare(null);
      setLegalMoves(Array(8).fill(Array(8).fill(false)));
    }
  };

  const handlePromotionSelect = (pieceChar) => {
    if (!pendingPromotion) return;
    executeMove(
      pendingPromotion.fromX, pendingPromotion.fromY, 
      pendingPromotion.destX, pendingPromotion.destY, 
      pendingPromotion.isCapture, 
      pieceChar
    );
    setPendingPromotion(null);
  };

  const handleDragStart = (e, x, y) => {
    if (gameState !== 0) { e.preventDefault(); return; }
    const clickedPiece = board[x][y];
    const isWhitePiece = clickedPiece !== 0 && String(clickedPiece) === String(clickedPiece).toUpperCase();
    const isCurrentTurnPiece = clickedPiece !== 0 && (
      (turn === 'white' && isWhitePiece) || 
      (turn === 'black' && !isWhitePiece)
    );
    
    if (isCurrentTurnPiece) fetchMovesForPiece(x, y);
    else e.preventDefault(); 
  };

  const handleSquareClick = (x, y) => {
    if (gameState !== 0) return;
    const clickedPiece = board[x][y];
    const isWhitePiece = clickedPiece !== 0 && String(clickedPiece) === String(clickedPiece).toUpperCase();
    const isCurrentTurnPiece = clickedPiece !== 0 && (
      (turn === 'white' && isWhitePiece) || 
      (turn === 'black' && !isWhitePiece)
    );

    if (isCurrentTurnPiece) fetchMovesForPiece(x, y);
    else handleMoveIntent(x, y);
  };

  const promoPieces = [
    { char: 'q', name: 'Queen' },
    { char: 'r', name: 'Rook' },
    { char: 'b', name: 'Bishop' },
    { char: 'n', name: 'Knight' }
  ];

  return (
    <div className="game-wrapper">
      
      {/* LEFT SIDE: The Board */}
      <div className="board" style={{ position: 'relative' }}>
        {board.map((row, x) => 
          row.map((piece, y) => {
            const isLight = (x + y) % 2 === 0;
            const isSelected = selectedSquare?.x === x && selectedSquare?.y === y;
            const isLegalDest = legalMoves[x][y];
            const hasEnemy = isLegalDest && piece !== 0;

            return (
              <div 
                key={`${x}-${y}`} 
                className={`square ${isLight ? 'light' : 'dark'} ${isSelected ? 'selected' : ''}`}
                onClick={() => handleSquareClick(x, y)}
                onDragOver={(e) => e.preventDefault()}
                onDrop={(e) => { e.preventDefault(); handleMoveIntent(x, y); }}
              >
                {isLegalDest && !hasEnemy && <div className="legal-dot"></div>}
                {isLegalDest && hasEnemy && <div className="legal-capture"></div>}
                
                {piece !== 0 && (
                  <div 
                    className="piece" 
                    style={getPieceStyle(piece)} 
                    draggable={gameState === 0} 
                    onDragStart={(e) => handleDragStart(e, x, y)}
                  ></div>
                )}
              </div>
            );
          })
        )}

        {/* PROMOTION POPUP OVERLAY */}
        {pendingPromotion && (
          <div className="promotion-overlay">
            <div className="promotion-box">
              <h3>Promote to:</h3>
              <div className="promo-options">
                {promoPieces.map(p => (
                  <div key={p.char} className="promo-card" onClick={() => handlePromotionSelect(p.char)}>
                    <div className="piece" style={getPieceStyle(pendingPromotion.color === 'w' ? p.char.toUpperCase() : p.char)}></div>
                    <span>{p.name}</span>
                  </div>
                ))}
              </div>
            </div>
          </div>
        )}

        {/* NEW FEATURE: GAME OVER SCREEN OVERLAY */}
        {gameState !== 0 && (
          <div className="game-over-overlay">
            <div className="game-over-box">
              {gameState === 1 && (
                <>
                  <h2>Checkmate!</h2>
                  <p>White wins by bringing down the enemy King.</p>
                </>
              )}
              {gameState === 2 && (
                <>
                  <h2>Checkmate!</h2>
                  <p>Black wins by bringing down the enemy King.</p>
                </>
              )}
              {gameState === 3 && (
                <>
                  <h2>Stalemate</h2>
                  <p>Draw: Player has no legal moves and is not in check.</p>
                </>
              )}
              {gameState === 4 && (
                <>
                  <h2>Draw Game</h2>
                  <p>The game ended in a draw (50-move rule or insufficient material).</p>
                </>
              )}
              <button className="restart-btn" onClick={handleRestart}>Play Again</button>
            </div>
          </div>
        )}
      </div>

      {/* RIGHT SIDE: Sidebar */}
      {/* RIGHT SIDE: Sidebar */}
      <div className="sidebar">
        <div className="turn-indicator">
          {gameState === 0 ? `${turn.toUpperCase()} TO MOVE` : 'GAME OVER'}
        </div>
        
        <div className="move-history">
          {moveHistory.map((move, index) => (
            <div key={index} className="move-entry">
              <span style={{opacity: 0.5, marginRight: '8px', fontSize: '0.9em'}}>
                {index % 2 === 0 ? `${Math.floor(index/2) + 1}.` : ''}
              </span>
              {move}
            </div>
          ))}
        </div>

        {/* NEW MANUAL RESET BUTTON IN SIDEBAR */}
        <button 
          className="restart-btn" 
          style={{ marginTop: '15px', backgroundColor: '#d32f2f' }} // Red color to signify a destructive action
          onClick={handleRestart}
        >
          Reset Game
        </button>
      </div>

    </div>
  );
}

export default App;