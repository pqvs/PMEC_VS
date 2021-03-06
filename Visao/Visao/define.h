#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
#define PI 3.14159265f
#define CI 1.12f
#define CD 0.82f
#define PWM_MAX 80
#define PWM_MIN 0
#define TEMPO_FRAME 13
#define COMPRIMENTO_CAMPO 150.0f //cm
#define LARGURA_CAMPO 130.0f //cm
#define BORDA_CAMPO 10.0f //cm
#define RAIO_ROBO (7.5f/2) //cm
#define TAMANHO_GOL 40.0f //cm
#define TAMANHO_HISTORICO 20 // u
#define VEL_MAX 32.0f // cm/s
#define VEL_MAX_FRAME 2.0f // cm/frame
#define RAIO_RODA_ROBO 2.0f // cm
#define W_RODA_MAX 20.0f // rad/s
#define SCALE_CAM (2470.0f/2550.0f) //  (Altura da camera - Altura do Rob�) / Altura da C�mera 
#define LARGURA_LINHA ((LARGURA_CAMPO/2.0f) - (LARGURA_CAMPO/2.0f)*(2500.0f/2550.0f))// Largura do campo * (Altura da camera - Altura da Borda) / Altura da C�mera