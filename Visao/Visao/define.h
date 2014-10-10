#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
#define PI 3.14159265f
#define CI 1.12f
#define CD 0.82f
#define PWM_MAX 80
#define PWM_MIN 35
#define TEMPO_FRAME 13
#define COMPRIMENTO_CAMPO 148.0f //cm
#define LARGURA_CAMPO 127.0f //cm
#define BORDA_CAMPO 10.0f //cm
#define RAIO_ROBO (7.5f/2) //cm
#define TAMANHO_GOL 37.0f //cm
#define TAMANHO_HISTORICO 20 // u
#define VEL_MAX 20.0f // cm/s
#define VEL_MAX_FRAME 5.0f // cm/frame
#define RAIO_RODA_ROBO 2.0f // cm
#define W_RODA_MAX 20.0f // rad/s
#define SCALE_CAM (1790.0f/1880.0f) //  (Altura da camera - Altura do Robô) / Altura da Câmera 
#define LARGURA_LINHA ((LARGURA_CAMPO/2.0f) - (LARGURA_CAMPO/2.0f)*(1875.0f/1880.0f))// Largura do campo * (Altura da camera - Altura da Borda) / Altura da Câmera