#include "ChessBoard.h"
using namespace std;

///////////////////////////////////////////////////////[CONSTRUCTORS]/////////////////////////////////////////////////////////////////////////////////////////////

ChessBoard::ChessBoard(size_t h, size_t w)
{
    height = h;
    width = w;
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    window = SDL_CreateWindow("ChessBoard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    bsurface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Soporta texturas para las piezas
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

ChessBoard::~ChessBoard()
{
    delete[] * pieceSet; // eliminamos las piezas
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
////////////////////////////////////////////////////[DRAW AND IMAGE FUNCTIONS]////////////////////////////////////////////////////////////////////////////////////////
void ChessBoard::drawBoard() // Añadir el highlight en un futuro[matriz binaria o numero de 64bits]
{

    SDL_RenderClear(renderer);
    bool cindex; // Color index
    for (int y = 0; y < CHESSHEIGHT; y++)
    {
        for (int x = 0; x < CHESSWIDTH; x++)
        {
            SDL_Rect tile;
            tile.x = x * width / CHESSWIDTH;
            tile.y = y * height / CHESSHEIGHT;
            tile.w = width / CHESSWIDTH;
            tile.h = height / CHESSHEIGHT;
            SDL_SetRenderDrawColor(renderer, tileColor[cindex].r, tileColor[cindex].g, tileColor[cindex].b, tileColor[cindex].a);
            cindex = !cindex;

            SDL_RenderFillRect(renderer, &tile);
        }
        cindex = !cindex; // Sift color lines
    }

    // SDL_RenderPresent(renderer);
}

void ChessBoard::loadSpritesheet(std::string path)
{
    spriteSheet = loadTexture(path.c_str(), renderer);
}

void ChessBoard::renderPiece(Piece *_piece)
{
    SDL_Rect renderQuad = {_piece->x, _piece->y, width / CHESSWIDTH, height / CHESSHEIGHT}; // Lugar de renderizado destino
    int xindex = 0, yindex = 0;
    switch (_piece->figure)
    {
    case KING:
        xindex = 0;
        break;
    case QUEEN:
        xindex = 1;
        break;
    case BISHOP:
        xindex = 2;
        break;
    case KNIGHT:
        xindex = 3;
        break;
    case ROOK:
        xindex = 4;
        break;
    case PAWN:
        xindex = 5;
        break;
    default:
        xindex - 1;
        cout << "No piece type set";
        break;
    }
    switch (_piece->color)
    {
    case WHITE:
        yindex = 0;
        break;
    case BLACK:
        yindex = 1;
    }
    SDL_Rect pieceClip = {PIECESIZE * xindex, PIECESIZE * yindex, PIECESIZE, PIECESIZE}; // Las xy determinaran la pieza elegida
    SDL_RenderCopy(renderer, spriteSheet, &pieceClip, &renderQuad);
}

void ChessBoard::setPiecesPos()
{
    for (int i = 0; i < PIECESETSIZE; i++)
    {
        if (pieceSet[i] != nullptr)
        {
            pieceSet[i]->x = pieceSet[i]->a * width / CHESSWIDTH; // Asignamos a cada ficha su sitio segun el tablero
            pieceSet[i]->y = height - ((pieceSet[i]->b + 1) * height / CHESSHEIGHT);
        }
    }
}

Piece *ChessBoard::pieceAt(Pos_t pos)
{

    for (int i = 0; i < PIECESETSIZE; i++)
    {
        if (pieceSet[i] != nullptr)
        {

            if (pieceSet[i]->a == pos.a && pieceSet[i]->b == pos.b)
            {
                return pieceSet[i];
            }
        }
    }
    // If nothing found return nullptr

    return nullptr;
}

Pos_t ChessBoard::tileAt(int x, int y) // devuelve la posicion de la ficha relativa al pixel
{

    Pos_t r = {floor((float)CHESSWIDTH * x / (float)width), CHESSHEIGHT - 1 - floor((float)CHESSWIDTH * y / (float)height)};
    return r;
}

void ChessBoard::drawPieces() ///////////Contruir de nuevo en base a lista de fichas
{
    // setPiecesPos(); // Computa las posiciones de las piezas (Mover en un futuro solo necesario cuando resizea o se mueve una pieza?)
    for (int i = 0; i < PIECESETSIZE; i++)
    {
        if (pieceSet[i] != nullptr)
        { // Si existe la pieza
            renderPiece(pieceSet[i]);
        }
    }
}

void ChessBoard::render()
{
    drawBoard();  // Dibuja Tablero
    drawPieces(); // Dibuja llas Piezas
    SDL_RenderPresent(renderer);
}

/////////////////////////////////////////////////////////////////////////////////[LOOP FUNCTION]//////////////////////////////////////////////////////////////////////

bool ChessBoard::loop()
{
    while (SDL_PollEvent(&event) != 0) // Vacia la pila de eventos
    {

        switch (event.type)
        {
        case SDL_QUIT:
            // Cerramos el programa
            return false;
            break;

        case SDL_WINDOWEVENT:
            wEventHandler(event); // Window event handler
            break;
            // Mose Related callbacks
        case SDL_MOUSEBUTTONDOWN:
            clickHandler(); // Inicia el proceso de seleción o arrastre de la pieza
            break;
        }
    }

    render(); // renderizamos el tablero

    return true;
}

/////////////////////////////////////[HANDLERS]//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ChessBoard::wEventHandler(SDL_Event &event)
{
    switch (event.window.event)
    {
    case SDL_WINDOWEVENT_SIZE_CHANGED:

        if (max(event.window.data1, event.window.data2) > width) // Permite empequeñecer comparando con el anterior
        {
            width = height = max(event.window.data1, event.window.data2); // Por ahora se queda cuadrado, si añadimos movimientos o alguna cosa cambiar
        }
        else if (min(event.window.data1, event.window.data2) < width)
        {
            width = height = min(event.window.data1, event.window.data2); // Por ahora se queda cuadrado, si añadimos movimientos o alguna cosa cambiar
        }

        SDL_SetWindowSize(window, width, height);
        setPiecesPos(); // Actualiza las Piezas
        break;

    case SDL_WINDOWEVENT_MINIMIZED:
        // DISABLE RENDER
        break;

    // Window maxized
    case SDL_WINDOWEVENT_MAXIMIZED:
        // ENABLE RENDER
        break;

    // Window restored
    case SDL_WINDOWEVENT_RESTORED:
        // ENABLE RENDER
        break;

    default:
        break;
    }
}

void ChessBoard::clickHandler()
{
    int mx, my;
    Pos_t destPos;
    SDL_GetMouseState(&mx, &my);
    tileSelect = tileAt(mx, my);
    Piece *selectedPiece = pieceAt(tileSelect);
    // Por ahora implementaremos sin tiempo si se suelta el raton en la pieza donde se pulso queda seleccionada la pieza
    if (selectedPiece != nullptr) // Si hemos cogido una Pieza
    {

        if (selectedPiece->color == state)
        { // Si hay pieza(Ojo Implementacion pocha facilona, el orden de los enum coincide)
            bool held = true;

            while (held != false)
            {

                SDL_PollEvent(&event);

                if (event.type == SDL_MOUSEMOTION) // Si se mueve actualiza la pantalla
                {
                    SDL_GetMouseState(&mx, &my);
                    selectedPiece->x = mx - (width / CHESSWIDTH) / 2; // El offset para agarrar por el centro
                    selectedPiece->y = my - (height / CHESSHEIGHT) / 2;
                    render(); // Sigue renderizando
                }

                if (event.type == SDL_MOUSEBUTTONUP) // Si la suelta inicia el proceso de validación de posicion y movimiento
                {
                    held = false;
                    SDL_GetMouseState(&mx, &my); // Acutaliza el raton
                    destPos = tileAt(mx, my);    // Ficha destino
                    if (isValidMove(selectedPiece, destPos))
                    {
                        if (pieceAt(destPos) == nullptr) // Si la casilla destino está libre y el movimento es valido
                        {
                            move(selectedPiece, destPos); // Mueve la pieza
                        }
                        else if (selectedPiece->color == pieceAt(destPos)->color) // Si hay una pieza  de su color devolverla a su sitio
                        {
                            // IMPLEMENTAR ENROQUE AQUI ES LA UNICA CONDICION QUE HACE ALGO AL SOLTAR UNA FICHA SOBRE OTRA
                            setPiecesPos(); // Reinicia las posiciones
                        }

                        else if (pieceAt(destPos)->color != selectedPiece->color)
                        {                                 // Si hay una pieza de otro color y el movimiento es valido matarla y tomar su posición
                            killPiece(pieceAt(destPos));  // elimina
                            move(selectedPiece, destPos); // Mueve la pieza
                        }
                    }
                    else
                    {

                        setPiecesPos(); // El movimiento no ha sido valido vuelve a su sitio
                    }
                }
            }
        }
    }
}
////////////////////////////////////////////////////////////////////[FUNCIONES DE JUEGO]//////////////////////////////////////////////////////////

void ChessBoard::newGame() // Positions pieces like classic game
{

    // Todo esto será borrado por la funcion loadFen() que simplificará cambiar el tablero

    Figure_t startPieces[8] = {ROOK, KNIGHT, BISHOP, KING, QUEEN, BISHOP, KNIGHT, ROOK};
    ////////WHITE///////////////////////////////////////
    for (int i = 0; i < 8; i++)
    {
        pieceSet[i] = new Piece(WHITE, startPieces[i], i, 0);
    }
    for (int i = 8; i < 16; i++)
    {
        pieceSet[i] = new Piece(WHITE, PAWN, i - 8, 1);
    }
    /////////////BLACK///////////////////////////////////
    for (int i = 16; i < 24; i++)
    {
        pieceSet[i] = new Piece(BLACK, startPieces[i - 16], i - 16, 7);
    }
    for (int i = 24; i < 32; i++)
    {
        pieceSet[i] = new Piece(BLACK, PAWN, i - 24, 6);
    }
    setPiecesPos();     // Actualiza las Piezas
    state = WHITEPLAYS; // Juegan Blancas
}

void ChessBoard::move(Piece *piece, Pos_t pos)
{
    piece->a = pos.a;
    piece->b = pos.b;
    piece->neverMoved = false;
    setPiecesPos(); // Actualizamos tablero
    nextTurn();     // Cambiamos al siguiente Jugador
}

void ChessBoard::killPiece(Piece *p)
{

    for (int i = 0; i < PIECESETSIZE; i++)
    {
        if (pieceSet[i] != nullptr)
        {
            if (pieceSet[i] == p) // SI se encuentra la Pieza en el tablero
            {
                delete pieceSet[i];
                pieceSet[i] = nullptr; // Se deja el slot libre
            }
        }
    }
}

bool ChessBoard::isValidMove(Piece *piece, Pos_t dest)
{
    // Preguntas:
    // Está la casilla destino en nuestro rango de acción(Direccion,distancia)
    // Nos Bloquea alguna ficha antes de llegar a dicha casilla?
    //(No tenemos en cuenta el color de las fichas el movimiento es valido si físicamente posible) Mas tarde se decide si se cancela porque es del mismo color
    // Pone esto a nuestro rey en jaque?????(Sencillamente comprobar está misma función con todas las piezas hacia el destino del rey y comprobar su color)

    // sacar direccion comprobar que es aplicable a la figura y buscar colisiones con otra figura
    // descartado ->
    // Funcion moveMap(Piece* p)
    // Toma una pieza y conociendo su posicion a,b  genera un mapa binario de trayectoria uint64_t(ojo deja de valer para tableros de dim arbitrarias)
    // Funcion posMap() -> devuelve un mapa con las posiciones
    // Interseccionando ambos

    if (isValidPieceMove(piece, dest)) // Mira si el movimiento es legal para la pieza
    {
        // Comprobamos el camino si hay obstaculos excepto en la casilla destino

        if (piece->figure != KNIGHT) // Excluir al caballo de este check ya que salta piezas y ademas no linealmente
        {
            Pos_t mov; // utilizado como vector direccional aqui dest-origen
            mov.a = dest.a - piece->a;
            mov.b = dest.b - piece->b;

            Pos_t relPos; // Posicion relativa

            for (int i = 0; i < PIECESETSIZE; i++)
            {
                if (pieceSet[i] != nullptr)
                { // Algoritmo 1: Comprueba si el producto escalar entre el vector desplazamiento y el vector posicion de la ficha respecto de la posicion origen es nulo usando un threshold OJO EVITAR QUE SEA NEGATIVO
                    // Sacamos posición relativa
                    relPos.a = pieceSet[i]->a - piece->a;
                    relPos.b = pieceSet[i]->b - piece->b;
                    // Calculamos modulos
                    float modRel = sqrt(relPos.a * relPos.a + relPos.b * relPos.b);
                    float modMov = sqrt(mov.a * mov.a + mov.b * mov.b);
                    if (modRel < modMov)
                    { // Ademas el modulo del movimiento ha de ser estrictamente menor al vector posición relativa pq la ficha ha de estar antes del destino

                        double prod = (float)(relPos.a * mov.a + relPos.b * mov.b);
                        double cos = prod / (modMov * modRel);
                        if (abs(prod / (modMov * modRel) - 1) <= FLOATCMPTHRESHOLD && prod > 0) // positivo para que la direccion sea la de menor angulo (Comprueba si la ficha en el rango de accion se encuentra en nuestra direccion) además evita que su propia posicion sea un movimiento valido producto por nulo es nulo
                        {
                            return false;
                        }
                    }
                }
            }

            return true; // Si sobrevive al check el movimiento se cataloga como valido
        }
        else if (piece->figure == KNIGHT)
        {
            return true; // El caballo no chequea y hay que darle paso
        }
    }
    else // Exit de movimiento no valido para pieza
    {
        return false;
    }
    return false; // Por diseño(No deberia ejecutarse nunca)VENIR A BUSCAR ERRORES AQUI(Piezas corruptas, existentes sin tipo)
}

bool ChessBoard::isValidPieceMove(Piece *piece, Pos_t dest) // Incluye las reglas de movimiento de todas las piezas, no controla colisiones
{
    switch (piece->figure)
    {
    case PAWN:
        switch (piece->color)
        {
        case WHITE:
            // Ojo comida diagonal, permitir movimiento diagonal y negar al no encontrar pieza mas tarde
            if ((dest.a == piece->a && (dest.b - piece->b == 1 || (dest.b - piece->b == 2 && piece->neverMoved))) && pieceAt(dest) == nullptr)
            {
                return true;
            }
            if (abs(dest.a - piece->a) == 1 && (dest.b - piece->b == 1) && pieceAt(dest) != nullptr)
            { // Comida diagonal
                return true;
            }
            break;

        case BLACK:
            // Ojo comida diagonal, permitir movimiento diagonal y negar al no encontrar pieza mas tarde
            if ((dest.a == piece->a && (dest.b - piece->b == -1 || (dest.b - piece->b == -2 && piece->neverMoved))) && pieceAt(dest) == nullptr) // Movimiento Normal
            {
                return true;
            }
            if (abs(dest.a - piece->a) == 1 && (dest.b - piece->b == -1) && pieceAt(dest) != nullptr)
            { // Comida diagonal
                return true;
            }

            break;
        }
        break;

    case ROOK:
        if (dest.a - piece->a == 0 || dest.b - piece->b == 0) // Ojo da por valida su propia casilla
        {
            return true;
        }

        break;

    case KNIGHT:
        if (((abs(dest.a - piece->a) == 2) && (abs(dest.b - piece->b) == 1)) || ((abs(dest.b - piece->b) == 2) && (abs(dest.a - piece->a) == 1)))
        {
            return true;
        }
        break;
    case BISHOP:
        if (abs(dest.a - piece->a) == abs(dest.b - piece->b))
        {
            return true;
        }
        break;

    case QUEEN:
        if ((abs(dest.a - piece->a) == abs(dest.b - piece->b)) || (dest.a - piece->a == 0 || dest.b - piece->b == 0))
        {

            return true;
        }
        break;
    case KING:
        if (sqrt(pow(abs(dest.a - piece->a), 2) + pow(abs(dest.b - piece->b), 2)) <= sqrt(2))
        {
            return true;
        }

        break;

        break;
        // Rey y reina

    default:
        break;
    }
    return false;
}

void ChessBoard::nextTurn()
{
    switch (state)
    {
    case WHITEPLAYS:
        state = BLACKPLAYS;
        break;

    case BLACKPLAYS:
        state = WHITEPLAYS;
        break;
    }
}