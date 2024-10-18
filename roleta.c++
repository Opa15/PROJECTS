#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <tchar.h>

// Definir constantes
#define ID_ROLAR 1
#define TAMANHO_RODA 300

// Prototipar a função
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// Função para desenhar a roleta
void DesenharRoda(HDC hdc, RECT rect, int resultado) {
    const int segmentos = 10;  // Quantos números na roleta
    int raio = (rect.right - rect.left) / 2;
    int centroX = (rect.right + rect.left) / 2;
    int centroY = (rect.bottom + rect.top) / 2;

    // Desenhar a roleta
    for (int i = 0; i < segmentos; ++i) {
        double anguloInicial = (i * 2.0 * 3.14159) / segmentos;
        double anguloFinal = ((i + 1) * 2.0 * 3.14159) / segmentos;

        HPEN hPen = CreatePen(PS_SOLID, 1, (i == resultado) ? RGB(255, 0, 0) : RGB(0, 0, 0)); // Cor para o segmento selecionado
        HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));
        SelectObject(hdc, hPen);
        SelectObject(hdc, hBrush);

        // Desenhar os segmentos da roleta
        Pie(hdc, centroX - raio, centroY - raio, centroX + raio, centroY + raio, 
            centroX + int(raio * cos(anguloInicial)), centroY + int(raio * sin(anguloInicial)),
            centroX + int(raio * cos(anguloFinal)), centroY + int(raio * sin(anguloFinal)));

        DeleteObject(hPen);
        DeleteObject(hBrush);

        // Desenhar os números
        TCHAR texto[3];
        _stprintf_s(texto, _T("%d"), i); // Converte o número i para texto
        SetTextColor(hdc, RGB(0, 0, 0));
        TextOut(hdc, centroX + int(raio * 0.7 * cos((anguloInicial + anguloFinal) / 2)) - 10,
                centroY + int(raio * 0.7 * sin((anguloInicial + anguloFinal) / 2)) - 10, texto, _tcslen(texto));
    }
}

// Função principal para a janela
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Inicializando e criando a janela
    const TCHAR szClassName[] = _T("Roleta");
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = szClassName;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, _T("Falha ao registrar a classe!"), _T("Erro"), MB_ICONERROR);
        return 0;
    }

    HWND hwnd = CreateWindowEx(
        0, szClassName, _T("Roleta Gráfica"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, _T("Falha ao criar a janela!"), _T("Erro"), MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Loop de mensagens
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

// Variável global para armazenar o resultado atual
int resultadoAtual = -1;

// Função de processamento da janela
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        srand(time(0));
        CreateWindow(_T("BUTTON"), _T("Girar Roleta"),
            WS_VISIBLE | WS_CHILD, 150, 320, 100, 30, hwnd, (HMENU)ID_ROLAR, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_ROLAR) {
            resultadoAtual = rand() % 10;  // Gera o resultado entre 0 e 9
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        DesenharRoda(hdc, rect, resultadoAtual);

        if (resultadoAtual != -1) {
            // Definir a porcentagem de vitória (por exemplo, 30%)
            int porcentagemGanho = 30;
            int sorteio = rand() % 100;

            TCHAR resultadoTexto[100];
            if (sorteio < porcentagemGanho) {
                _stprintf_s(resultadoTexto, _T("Você ganhou!"));
            } else {
                _stprintf_s(resultadoTexto, _T("Você perdeu!"));
            }

            SetTextColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, 150, 360, resultadoTexto, _tcslen(resultadoTexto));
        }

        EndPaint(hwnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
