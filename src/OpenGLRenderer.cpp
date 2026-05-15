/*
* Copyright (c) 2026 Tim Bruckhoff / Affiliation: Ruprecht-Karls-University Heidelberg, Germany
*
* This source code is licensed under the EUPL v1.2.
* See the LICENSE file in the repository root directory for full details.
*
* CITATION:
* If you use this software for your research, please cite it as
* described in the CITATION.cff file located in the root directory.
*
* SPDX-License-Identifier: EUPL-1.2
*/

#include "pch.h"
#include "CUVNMR.h"
#include "OpenGlRenderer.h"
#include "ViewPropertiesDlg.h"
#include "MoleculeViewerDlg.h"
#include "CuvetteDlg.h"
#include "CSetNameDlg.h"
#include <sstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(OpenGlRenderer, CWnd)
OpenGlRenderer::OpenGlRenderer()
{
    m_epsilon = 0.00001;
    m_iViewPortWidth = 200;
    m_iViewPortHeight = 200;
    m_hdc = NULL;
    m_r_mouse_button_down = false;
    m_r_mouse_start_radius = 1;
    m_primary_view = false;
}

OpenGlRenderer::~OpenGlRenderer()
{
    DestroyScene();
}

BEGIN_MESSAGE_MAP(OpenGlRenderer, CWnd)
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_MENU0_ADDSELECTEDASGROUP, &OpenGlRenderer::OnAddSelectedAsGroup)
    ON_COMMAND(ID_MENU0_ADDSELECTEDASINDIVIDUALS, &OpenGlRenderer::OnAddSelectedAsIndividuals)
    ON_COMMAND(ID_MENU0_VIEWPROPERTIES, &OpenGlRenderer::OnViewProperties)
    ON_COMMAND(ID_MENU0_DESELECT_ALL, &OpenGlRenderer::OnMenu0DeselectAll)
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_SETCURSOR()
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEHWHEEL()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM) { return 0; }
HDC get_dc(HWND hwnd) { return GetDC(hwnd); }
void release_dc(HWND hwnd, HDC hdc) { ReleaseDC(hwnd, hdc); }
void destroy_window(HWND hwnd) { DestroyWindow(hwnd); }

bool OpenGlRenderer::CreateGLContext(CRect rect, LPCWSTR classNameStr, CWnd* parent)
{
    CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(WHITE_BRUSH), NULL);
    CreateEx(0, className, _T("OpenGL with MFC/CDialog"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);
    SetViewPortSize(rect.Width(), rect.Height());
    m_WindowOld = rect;
    m_WindowOrig = rect;
    HGLRC hrc;
    HDC hdc;
    HWND hwnd;
    HINSTANCE hInstance = GetModuleHandle(NULL);  
    m_wc_ex.cbSize = sizeof(WNDCLASSEX);
    m_wc_ex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    m_wc_ex.lpfnWndProc = WndProc;
    m_wc_ex.cbClsExtra = 0;
    m_wc_ex.cbWndExtra = 0;
    m_wc_ex.hInstance = hInstance;
    m_wc_ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    m_wc_ex.hCursor = LoadCursor(NULL, IDC_ARROW);
    m_wc_ex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    m_wc_ex.lpszMenuName = NULL;
    m_wc_ex.lpszClassName = classNameStr;
    m_wc_ex.hIconSm = NULL;
    m_class_name_str = classNameStr;
    if (!RegisterClassEx(&m_wc_ex))
    {
        return 0;
    }
    hwnd = CreateWindow(
        (LPWSTR)"coco",
        (LPWSTR)"dddd",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 500,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    hdc = get_dc(hwnd);
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, nPixelFormat, &pfd);
    hrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hrc);
    glewExperimental = true;
    glewInit();
    if (wglewIsSupported("WGL_ARB_create_context") == 1)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hrc);
        release_dc(hwnd, hdc);
        destroy_window(hwnd);
        m_hdc = GetDC()->m_hDC;
        const int iPixelFormatAttribList[] = 
		{
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 24,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB, 4,
            0
        };
        int attributes[] = 
		{
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3
            , WGL_CONTEXT_MINOR_VERSION_ARB, 3
            , WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
            , 0
        };
        int nPixelFormat = 0;
        UINT iNumFormats = 0;
        wglChoosePixelFormatARB(m_hdc, iPixelFormatAttribList, NULL, 1, &nPixelFormat, (UINT*)&iNumFormats);
        SetPixelFormat(m_hdc, nPixelFormat, &pfd);
        m_hrc = wglCreateContextAttribsARB(m_hdc, 0, attributes);
        CreateRGBPalette(m_hdc);
        wglMakeCurrent(NULL, NULL);
        wglMakeCurrent(m_hdc, m_hrc);
    }
    init_gl();
    m_data_base->getCamera()->resize(rect.Width(), rect.Height());
    wglMakeCurrent(NULL, NULL);
    return true;
}

BOOL OpenGlRenderer::CreateRGBPalette(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int n = GetPixelFormat(hDC);
    DescribePixelFormat(hDC, n, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    if (!(pfd.dwFlags & PFD_NEED_PALETTE)) return FALSE;
    LOGPALETTE* pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE)
        + 256 * sizeof(PALETTEENTRY));
    if (!pPal)
    {
        AfxMessageBox(TEXT("Out of memory for pallet"), MB_OK | MB_ICONSTOP);
        return FALSE;
    }
    pPal->palVersion = 0x300;
    pPal->palNumEntries = 256;
    ASSERT(pfd.cColorBits == 8);
    n = 1 << pfd.cColorBits;
    for (int i = 0; i < n; i++)
    {
        pPal->palPalEntry[i].peRed =
            ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
        pPal->palPalEntry[i].peGreen =
            ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
        pPal->palPalEntry[i].peBlue =
            ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
        pPal->palPalEntry[i].peFlags = 0;
    }
    if ((pfd.cColorBits == 8) &&
        (pfd.cRedBits == 3) && (pfd.cRedShift == 0) &&
        (pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
        (pfd.cBlueBits == 2) && (pfd.cBlueShift == 6)
        )
    {
        for (int j = 1; j <= 12; j++)
            pPal->palPalEntry[m_defaultOverride[j]] = m_defaultPalEntry[j];
    }
    if (m_pPal)
        delete m_pPal;
    m_pPal = new CPalette;
    BOOL bResult = m_pPal->CreatePalette(pPal);
    free(pPal);
    return bResult;
}

unsigned char OpenGlRenderer::m_threeto8[8] = {0, 0111 >> 1, 0222 >> 1, 0333 >> 1, 0444 >> 1, 0555 >> 1, 0666 >> 1, 0377};
unsigned char OpenGlRenderer::m_twoto8[4] = {0, 0x55, 0xaa, 0xff};
unsigned char OpenGlRenderer::m_oneto8[2] = {0, 255};
int OpenGlRenderer::m_defaultOverride[13] = {0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91};

PALETTEENTRY OpenGlRenderer::m_defaultPalEntry[20] = 
{
    { 0,   0,   0,    0 },
    { 0x80,0,   0,    0 },
    { 0,   0x80,0,    0 },
    { 0x80,0x80,0,    0 },
    { 0,   0,   0x80, 0 },
    { 0x80,0,   0x80, 0 },
    { 0,   0x80,0x80, 0 },
    { 0xC0,0xC0,0xC0, 0 },
    { 192, 220, 192,  0 },
    { 166, 202, 240,  0 },
    { 255, 251, 240,  0 },
    { 160, 160, 164,  0 },
    { 0x80,0x80,0x80, 0 },
    { 0xFF,0,   0,    0 },
    { 0,   0xFF,0,    0 },
    { 0xFF,0xFF,0,    0 },
    { 0,   0,   0xFF, 0 },
    { 0xFF,0,   0xFF, 0 },
    { 0,   0xFF,0xFF, 0 },
    { 0xFF,0xFF,0xFF, 0 }
};

unsigned char OpenGlRenderer::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
    unsigned char val;
    val = (unsigned char)(i >> shift);
    switch (nbits) 
	{
    case 1:
        val &= 0x1;
        return m_oneto8[val];
    case 2:
        val &= 0x3;
        return m_twoto8[val];
    case 3:
        val &= 0x7;
        return m_threeto8[val];
    default:
        return 0;
    }
}

void OpenGlRenderer::PrepareScene()
{
    wglMakeCurrent(m_hdc, m_hrc);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    wglMakeCurrent(NULL, NULL);
}

void OpenGlRenderer::Reshape(UINT nType, int w, int h)
{
    OnSize(nType, w, h);
}

void OpenGlRenderer::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    if (m_hdc != NULL)
    {
        wglMakeCurrent(m_hdc, m_hrc);
        glViewport(0, 0, (GLsizei)cx, (GLsizei)cy);
        SetViewPortSize(cx, cy);
        switch (nType)
        {
        case SIZE_RESTORED:
        case SIZE_MAXIMIZED:
        {
            MoveWindow(m_WindowOld.left, m_WindowOld.top, GetViewPortWidth(), GetViewPortHeight(), TRUE);
            break;
        }
        default:
            break;
        }
    }
    wglMakeCurrent(NULL, NULL);
}

void OpenGlRenderer::DrawScene()
{
    CPaintDC dc(this);
    CPalette* ppalOld = NULL;
    if (m_pPal)
    {
        ppalOld = dc.SelectPalette(m_pPal, 0);
        dc.RealizePalette();
    }
    int width, height;
    GetSize(width, height);
    m_data_base->getCamera()->resize(width, height);
    BOOL bResult = wglMakeCurrent(dc.m_hDC, m_hrc);
    glm::vec3 bg_col = m_data_base->getBackgroundColor();
    glClearColor(bg_col.r, bg_col.g, bg_col.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setup_rendering();
    draw_gl();
    glFlush();
    SwapBuffers(dc.m_hDC);
    if (ppalOld)
        dc.SelectPalette(ppalOld, 0);
    wglMakeCurrent(NULL, NULL);
}

void OpenGlRenderer::DestroyScene()
{
    DestroyWindow();
    wglMakeCurrent(NULL, NULL);
    if (m_hrc)
    {
        wglDeleteContext(m_hrc);
        m_hrc = NULL;
    }
    int ret = UnregisterClass(m_class_name_str, m_wc_ex.hInstance);
    if (ret == 0)
    {
        int a = 0;
    }
}

void OpenGlRenderer::SetViewPortSize(int ViewPortWidth, int ViewPortHeight)
{
    m_iViewPortWidth = ViewPortWidth;
    m_iViewPortHeight = ViewPortHeight;
    m_data_base->getCamera()->resize(ViewPortWidth, ViewPortHeight);
};

void OpenGlRenderer::LoadShaders() {
    std::string VertexShaderCode = R"asdafadfad(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNorm;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    out vec3 pos_vs;
    out vec3 norm_vs;
    void main()
    {
        pos_vs = (model * vec4(aPos, 1.0f)).xyz;
        norm_vs = (model * vec4(aNorm, 0.0f)).xyz;
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
    }
    )asdafadfad";
    std::string FragmentShaderCode = R"asdafadfad(
    #version 330 core
    out vec4 FragColor;
    in vec3 pos_vs;
    in vec3 norm_vs;
    uniform vec3 model_color;
    uniform vec3 light_dir;
    void main()
    {   
        vec3 normal = normalize(norm_vs);
		float diff = max(0.5*(dot(normalize(light_dir), normal) + 1), 0.0);
		float shade = 0.8 * diff + 0.2;
		FragColor.rgb = shade * model_color;
		FragColor.a = 1.0f;
    }
    )asdafadfad";
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLint Result = GL_FALSE;
    int InfoLogLength;
    printf("Compiling Vertex Shader\n");
    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }
    printf("Compiling Fragment Shader\n");
    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    m_shader_id = ProgramID;
    m_data_base->getCamera()->attach_shader(ProgramID);
    return;
}

void OpenGlRenderer::init_sphere() 
{
    // Ref: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
    glGenVertexArrays(1, &m_vao_sphere);
    glBindVertexArray(m_vao_sphere);
    std::vector<glm::vec3> normals;
    int N = 100;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float x1 = (i * 2 * glm::pi<float>()) / N;
            float y1 = (j * glm::pi<float>()) / N;
            float x2 = (((i + 1) % N) * 2 * glm::pi<float>()) / N;
            float y2 = ((j + 1) % N * glm::pi<float>()) / N;
            glm::vec3 P00(sin(x1) * sin(y1), cos(x1) * sin(y1), cos(y1));
            glm::vec3 P01(sin(x1) * sin(y2), cos(x1) * sin(y2), cos(y2));
            glm::vec3 P10(sin(x2) * sin(y1), cos(x2) * sin(y1), cos(y1));
            glm::vec3 P11(sin(x2) * sin(y2), cos(x2) * sin(y2), cos(y2));
            m_sphere_vertices.emplace_back(P00);
            m_sphere_vertices.emplace_back(P01);
            m_sphere_vertices.emplace_back(P11);
            m_sphere_vertices.emplace_back(P00);
            m_sphere_vertices.emplace_back(P11);
            m_sphere_vertices.emplace_back(P10);
            normals.emplace_back(P00);
            normals.emplace_back(P01);
            normals.emplace_back(P11);
            normals.emplace_back(P00);
            normals.emplace_back(P11);
            normals.emplace_back(P10);
        }
    }
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(m_sphere_vertices.size() * sizeof(glm::vec3)), &m_sphere_vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(normals.size() * sizeof(glm::vec3)), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
}

void OpenGlRenderer::init_cylinder() 
{

    // Ref: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
    glGenVertexArrays(1, &m_vao_cylinder);
    glBindVertexArray(m_vao_cylinder);
    std::vector<glm::vec3> normals;
    int N = 100;
    for (int i = 0; i < N; i++) 
	{
        float x1 = (i * 2 * glm::pi<float>()) / N;
        float x2 = (((i + 1) % N) * 2 * glm::pi<float>()) / N;
        glm::vec3 P00(sin(x1), cos(x1), -0.5);
        glm::vec3 P01(sin(x1), cos(x1), 0.5);
        glm::vec3 P10(sin(x2), cos(x2), -0.5);
        glm::vec3 P11(sin(x2), cos(x2), 0.5);
        m_cylinder_vertices.emplace_back(P00);
        m_cylinder_vertices.emplace_back(P01);
        m_cylinder_vertices.emplace_back(P11);
        m_cylinder_vertices.emplace_back(P00);
        m_cylinder_vertices.emplace_back(P11);
        m_cylinder_vertices.emplace_back(P10);
        glm::vec3 N00(sin(x1), cos(x1), 0);
        glm::vec3 N01(sin(x1), cos(x1), 0);
        glm::vec3 N10(sin(x2), cos(x2), 0);
        glm::vec3 N11(sin(x2), cos(x2), 0);
        normals.emplace_back(N00);
        normals.emplace_back(N01);
        normals.emplace_back(N11);
        normals.emplace_back(N00);
        normals.emplace_back(N11);
        normals.emplace_back(N10);
    }
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(m_cylinder_vertices.size() * sizeof(glm::vec3)), &m_cylinder_vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(normals.size() * sizeof(glm::vec3)), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
}

void OpenGlRenderer::init_light() 
{
    m_light_dir_id = glGetUniformLocation(m_shader_id, "light_dir");
    glUniform3f(m_light_dir_id, 1.0f, 1.0f, 1.0f);
}

void OpenGlRenderer::init_gl() 
{
    CString str;
    str.Format(_T("OpenGL version: %s\n"), (CString)glGetString(GL_VERSION));
    TRACE(str);
    LoadShaders();
    glUseProgram(m_shader_id);
    // Ref: https://learnopengl.com/Advanced-OpenGL/Depth-testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    init_sphere();
    init_cylinder();
    m_data_base->getCamera()->init();
    init_light();
    m_model_id = glGetUniformLocation(m_shader_id, "model");
    m_model_color_id = glGetUniformLocation(m_shader_id, "model_color");
}

void OpenGlRenderer::setup_rendering() 
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glUseProgram(m_shader_id);
    m_data_base->getCamera()->setup();
}

void OpenGlRenderer::draw_gl() 
{
    draw_points();
    draw_cylinders();
}

void OpenGlRenderer::draw_points() 
{
    int num = m_data_base->getNumPoints();
    for (int i = 0; i < num; i++) 
	{
        draw_point(m_data_base->getPoint(i));
    }
}

void OpenGlRenderer::draw_point(Point* pnt) 
{
    glBindVertexArray(m_vao_sphere);
    glm::vec3 color = pnt->name->getColor();
    if (pnt->isSelected) 
	{
        glUniform3f(m_model_color_id, 234.0f/255.0f, 153.0f/255.0f, 218.0f/255.0f);
    }
    else 
	{
        glUniform3f(m_model_color_id, color.r, color.g, color.b);
    }
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pnt->pos);
    model = glm::scale(model, glm::vec3(pnt->name->getRadius(),
        pnt->name->getRadius(), pnt->name->getRadius()));
    glUniformMatrix4fv(m_model_id, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, m_sphere_vertices.size());
}

void OpenGlRenderer::draw_cylinder(Point* pntA, Point* pntB, float dist) 
{
    glBindVertexArray(m_vao_cylinder);
    glm::vec3 color = { 0.2f, 0.2f, 0.2f };
    glUniform3f(m_model_color_id, color.r, color.g, color.b);
    glm::vec3 pos = 0.5f * (pntA->pos + pntB->pos);
    glm::vec3 dir = glm::normalize(pntA->pos - pntB->pos);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, -glm::pi<float>() / 2, glm::vec3(0, 1, 0));
    float yaw = atan2(dir.y, dir.x);
    model = glm::rotate(model, yaw, glm::vec3(1, 0, 0));
    float pitch = asin(-dir.z);
    model = glm::rotate(model, pitch, glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(m_data_base->getCylinderRadius(), m_data_base->getCylinderRadius(), dist));
    glUniformMatrix4fv(m_model_id, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, m_cylinder_vertices.size());
}

void OpenGlRenderer::draw_cylinders() 
{
    int num = m_data_base->getNumPoints();
    for (int i = 0; i < num; i++) 
	{
        Point* pntA = m_data_base->getPoint(i);
        for (int j = i; j < num; j++) 
		{
            Point* pntB = m_data_base->getPoint(j);
            PointConnection* conn = m_data_base->getConnection(pntA->name->getIndex(), pntB->name->getIndex());
            float dist = glm::distance(pntA->pos, pntB->pos);
            if (dist > 0.00001) 
			{
                if (dist < conn->getThreshold()) 
				{
                    draw_cylinder(pntA, pntB, dist);
                }
            }
        }
    }
}

void OpenGlRenderer::GetSize(int& p_width, int& p_height)
{
    RECT rect;
    GetClientRect(&rect);
    p_width = rect.right;
    p_height = rect.bottom;
}

void OpenGlRenderer::OnPaint()
{
    DrawScene();
}

void OpenGlRenderer::OnViewProperties() 
{
    if (m_primary_view) 
    {
        return;
    }
    ViewPropertiesDlg dlg(m_data_base);
    INT_PTR nResponse = dlg.DoModal();
    Invalidate();
    UpdateWindow();
}

void OpenGlRenderer::OnCloseWindow() 
{
    if (m_primary_view) 
	{
        return;
    }
}

void OpenGlRenderer::attach_data_base(DataBase* data_base) 
{
    m_data_base = data_base;
    int num = m_data_base->getNumPoints();
    float dist = 0;
    for (int i = 0; i < num; i++) 
	{

        Point* pnt = m_data_base->getPoint(i);
        dist += glm::distance(pnt->pos, m_data_base->getCamera()->getPos());
    }
    m_r_mouse_start_radius = dist / num;
}

void OpenGlRenderer::setPrimary() 
{
    m_primary_view = true;
}

int OpenGlRenderer::FormattedErrorAfxMsgBox(const char* p_msg)
{
    std::stringstream str;
    str << p_msg;
    CComBSTR bstr(str.str().c_str());
    AfxMessageBox(bstr, MB_OK | MB_ICONSTOP);
    return -1;
}

void OpenGlRenderer::setList(CListCtrl* list_main) 
{
    g_list_main = list_main;
}

void OpenGlRenderer::OnMenu0DeselectAll()
{
    int num = m_data_base->getNumPoints();
    for (int i = 0; i < num; i++)
    {
        Point* pnt = m_data_base->getPoint(i);
        pnt->isSelected = false;
    }
    Invalidate();
    UpdateWindow();
}

void OpenGlRenderer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonDblClk(nFlags, point);
}

void OpenGlRenderer::OnAddSelectedAsIndividuals() 
{
    if (m_primary_view) 
    {
        return;
    }
    int nItem;
    int num = m_data_base->getNumPoints();
    std::wstring str_group_name = L"unnamed";
    for (int i = 0; i < num; i++) 
    {
        Point* pnt = m_data_base->getPoint(i);
        if (pnt->isSelected) 
        {
            bool found = m_data_base->m_simulation.IndexListed(i);
            if (!found) 
            {
                if (str_group_name == L"unnamed")
                {
                    CSetNameDlg dlg;
                    if (dlg.DoModal() == IDOK)
                    {
                        str_group_name = dlg.GetInputText();
                    }
                }
                m_data_base->m_simulation.AddToList(str_group_name, { i }, pnt->name->getName(), 0.0, 0.0, false);
            }
            else 
            {
                std::string msg = "Atom " + std::to_string(i);
                std::string name(pnt->name->getName().begin(), pnt->name->getName().end());
                msg = msg + " (" + name.c_str();
                msg = msg + ") already exists in the list.";
                FormattedErrorAfxMsgBox(msg.c_str());
            }
            pnt->isSelected = false;
        }
    }
    Invalidate();
    UpdateWindow();
}

void OpenGlRenderer::OnAddSelectedAsGroup() 
{
    if (m_primary_view)
    {
        return;
    }
    int nItem;
    int num = m_data_base->getNumPoints();
    bool cancel = false;
    bool cancel2 = false;
    bool a_point_was_selected = false;
    std::vector<int> indices;
    PointName* name = nullptr;
    for (int i = 0; i < num; i++)
    {
        Point* pnt = m_data_base->getPoint(i);
        if (pnt->isSelected)
        {
            bool found = m_data_base->m_simulation.IndexListed(i); // überprüfen, ob dieser Punkt schonmal in der Liste vorkommt
            if (!found)
            {
                indices.push_back(i);
                if (name != nullptr)
                {
                    if (name != pnt->name)
                    {
                        cancel2 = true;
                    }
                }
                else
                {
                    name = pnt->name;
                    a_point_was_selected = true;
                }
            }
            else
            {
                std::wstring str_name = pnt->name->getName().c_str();
                std::string msg = "Atom " + std::to_string(i);
                std::string s_name(str_name.begin(), str_name.end());
                msg = msg + " (" + s_name.c_str();
                msg = msg + ") already exists in the list.";
                FormattedErrorAfxMsgBox(msg.c_str());
                cancel = true;
            }
            pnt->isSelected = false;
        }
    }
    if (!cancel && !cancel2 && a_point_was_selected)
    {
        std::wstring str_group_name = L"unnamed";
        if (str_group_name == L"unnamed")
        {
            CSetNameDlg dlg;
            if (dlg.DoModal() == IDOK)
            {
                str_group_name = dlg.GetInputText();
            }
        }
        m_data_base->m_simulation.AddToList(str_group_name, indices, name->getName(), 0.0, 0.0, false);
    }
    else
    {
        if (a_point_was_selected && !cancel && cancel2)
        {
            AfxMessageBox(_T("Selected atoms must be of same element."), MB_ICONWARNING);
        }
    }
    Invalidate();
    UpdateWindow();
}

BOOL OpenGlRenderer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (m_primary_view)
    {
        return CWnd::OnMouseWheel(nFlags, zDelta, pt);
    }
    m_data_base->getCamera()->moveForwards(0.1 * zDelta / m_r_mouse_start_radius);
    Invalidate();
    UpdateWindow();
    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void OpenGlRenderer::OnLButtonDown(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonDown(nFlags, point);
    if (m_primary_view) 
	{
        CMoleculeViewerDlg dlg(m_data_base);
        dlg.setList(g_list_main);
        INT_PTR nResponse = dlg.DoModal();
        Invalidate();
        UpdateWindow();
        return;
    }
    int width, height;
    GetSize(width, height);
    Ray r = m_data_base->getCamera()->transform(point.x, point.y, width, height);
    float minDist = FLT_MAX;
    int minInd = -1;
    int num = m_data_base->getNumPoints();
    for (int i = 0; i < num; i++) 
	{
        Point* pnt = m_data_base->getPoint(i);
        float dist = r.pointDistance(pnt->pos);
        if (dist < pnt->name->getRadius()) 
		{
            if (dist < minDist) 
			{
                minDist = dist;
                minInd = i;
            }
        }
    }
    if (minInd >= 0) 
	{
        (m_data_base->getPoint(minInd))->isSelected = !(m_data_base->getPoint(minInd))->isSelected;
        Invalidate();
        UpdateWindow();
    }
}

void OpenGlRenderer::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_r_mouse_rotation_done = false;
    CWnd::OnRButtonDown(nFlags, point);
    if (m_primary_view) 
    {
        return;
    }
    m_r_mouse_start_ang = glm::vec2(m_data_base->getCamera()->getPitch(), m_data_base->getCamera()->getRoll());
    CPaintDC dc(this);
    BOOL bResult = wglMakeCurrent(dc.m_hDC, m_hrc);
    if (!bResult) 
    {
        return;
    }
    int width, height;
    GetSize(width, height);
    const DWORD pos = ::GetMessagePos();
    m_r_mouse_start_pos = glm::vec2(int(LOWORD(pos)), int(HIWORD(pos)));
    if (GetKeyState(VK_SHIFT) & 0x8000)
    {
        m_r_mouse_mode_rot = false;
    }
    else 
    {
        m_r_mouse_mode_rot = true;
    }
    m_r_mouse_button_down = true;
}

void OpenGlRenderer::OnRButtonUp(UINT nFlags, CPoint point)
{
    CWnd::OnRButtonUp(nFlags, point);
    if (m_primary_view) 
    {
        return;
    }
    m_r_mouse_button_down = false;
}

BOOL OpenGlRenderer::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_primary_view)
    {
        return CWnd::OnSetCursor(pWnd, nHitTest, message);
    }
    if (m_r_mouse_button_down)
    {
        updateDrag();
    }
    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void OpenGlRenderer::updateDrag() {

    CPaintDC dc(this);
    BOOL bResult = wglMakeCurrent(dc.m_hDC, m_hrc);
    if (!bResult) 
    {
        return;
    }
    int width, height;
    GetSize(width, height);
    m_data_base->getCamera()->resize(width, height);
    const DWORD pos = ::GetMessagePos();
    glm::vec2 cur(int(LOWORD(pos)), int(HIWORD(pos)));
    if (m_r_mouse_mode_rot)
    {
        m_data_base->RotatePoints(m_data_base->getCamera()->CalculateDisplacementAlongY(cur, m_r_mouse_start_pos), m_data_base->getCamera()->CalculateDisplacementAlongX(cur, m_r_mouse_start_pos));
        if (cur != m_r_mouse_start_pos)
        {
            m_r_mouse_rotation_done = true;
        }
    }
    m_r_mouse_start_pos = cur;
    Invalidate();
    UpdateWindow();
}

void OpenGlRenderer::OnContextMenu(CWnd* pWnd, CPoint point)
{
    if (m_primary_view)
    {
        return;
    }
    const DWORD pos = ::GetMessagePos();
    glm::vec2 cur(int(LOWORD(pos)), int(HIWORD(pos)));
    float dist = glm::distance(m_r_mouse_start_pos, cur);
    if (dist > 3)
    {
        return;
    }
    if (m_r_mouse_rotation_done)
    {
        return;
    }
    if (point.x == -1 && point.y == -1)
    {
        return;
    }
    CMenu menu;
    menu.CreatePopupMenu();
    menu.AppendMenu(MF_STRING, ID_MENU0_ADDSELECTEDASGROUP, L"Add selected as group");
    menu.AppendMenu(MF_STRING, ID_MENU0_ADDSELECTEDASINDIVIDUALS, L"Add selected as inididuals");
    menu.AppendMenu(MF_STRING, ID_MENU0_VIEWPROPERTIES, L"View properties");
    menu.AppendMenu(MF_STRING, ID_MENU0_DESELECT_ALL, L"Deselect All");
    MENUINFO mi = { sizeof(MENUINFO) };
    mi.fMask = MIM_MAXHEIGHT;
    mi.cyMax = 400;
    menu.SetMenuInfo(&mi);
    menu.TrackPopupMenu(0, point.x, point.y, this);
}