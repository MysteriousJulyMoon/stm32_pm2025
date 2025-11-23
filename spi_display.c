#include <pic18f4620.h>
#include <stdint.h>

// Определения пинов для SPI
#define CS_PIN      PORTAbits.RA0
#define DC_PIN      PORTAbits.RA1
#define RESET_PIN   PORTAbits.RA2
#define CS_TRIS     TRISAbits.TRISA0
#define DC_TRIS     TRISAbits.TRISA1
#define RESET_TRIS  TRISAbits.TRISA2

// Прототипы функций
void SPI_Init(void);
void SPI_Write(uint8_t data);
void SSD1306_Command(uint8_t cmd);
void SSD1306_Data(uint8_t data);
void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_SetPosition(uint8_t page, uint8_t column);
void SSD1306_ChessBoard(void);
void SSD1306_TestPattern(void);
void delay_ms(unsigned int ms);

// Инициализация SPI
void SPI_Init(void) {
    // Настройка пинов управления
    CS_TRIS = 0;    // CS как выход
    DC_TRIS = 0;    // DC как выход
    RESET_TRIS = 0; // RESET как выход
    
    CS_PIN = 1;     // CS неактивен (HIGH)
    DC_PIN = 0;     // DC по умолчанию для команд
    RESET_PIN = 1;  // RESET неактивен
    
    // Настройка SPI модуля
    SSPSTAT = 0x40; // CKE = 1, SMP = 0
    SSPCON1 = 0x20; // SPI Master, Fosc/64
}

// Отправка байта по SPI
void SPI_Write(uint8_t data) {
    SSPBUF = data;          // Запись данных в буфер
    while (!SSPSTATbits.BF); // Ожидание завершения передачи
}

// Отправка команды дисплею
void SSD1306_Command(uint8_t cmd) {
    CS_PIN = 0;     // Активировать чип
    DC_PIN = 0;     // Режим команды
    SPI_Write(cmd);
    CS_PIN = 1;     // Деактивировать чип
}

// Отправка данных дисплею
void SSD1306_Data(uint8_t data) {
    CS_PIN = 0;     // Активировать чип
    DC_PIN = 1;     // Режим данных
    SPI_Write(data);
    CS_PIN = 1;     // Деактивировать чип
}

// Аппаратный сброс дисплея
void SSD1306_Reset(void) {
    RESET_PIN = 0;  // Активный сброс
    delay_ms(10);
    RESET_PIN = 1;  // Нормальная работа
    delay_ms(10);
}

// Инициализация дисплея SSD1306
void SSD1306_Init(void) {
    SSD1306_Reset(); // Аппаратный сброс
    
    // Последовательность команд инициализации
    SSD1306_Command(0xAE); // Display OFF (sleep mode)
    
    SSD1306_Command(0x20); // Set Memory Addressing Mode
    SSD1306_Command(0x00); // Horizontal addressing mode
    
    SSD1306_Command(0x21); // Set column address
    SSD1306_Command(0x00); // Column start address
    SSD1306_Command(0x7F); // Column end address (127)
    
    SSD1306_Command(0x22); // Set page address
    SSD1306_Command(0x00); // Page start address
    SSD1306_Command(0x07); // Page end address (7 pages для 64px)
    
    SSD1306_Command(0x40); // Set display start line
    
    SSD1306_Command(0xA1); // Set segment re-map (0xA0 - normal, 0xA1 - remap)
    SSD1306_Command(0xC8); // Set COM Output Scan Direction (0xC0 - normal, 0xC8 - remap)
    
    SSD1306_Command(0x81); // Set contrast control
    SSD1306_Command(0xFF); // Contrast value
    
    SSD1306_Command(0xA6); // Set normal display (0xA6 - normal, 0xA7 - inverse)
    
    SSD1306_Command(0xA8); // Set multiplex ratio
    SSD1306_Command(0x3F); // 64MUX
    
    SSD1306_Command(0xD3); // Set display offset
    SSD1306_Command(0x00); // No offset
    
    SSD1306_Command(0xD5); // Set display clock divide ratio/oscillator frequency
    SSD1306_Command(0x80); // Default
    
    SSD1306_Command(0xD9); // Set pre-charge period
    SSD1306_Command(0x22); // Default
    
    SSD1306_Command(0xDA); // Set com pins hardware configuration
    SSD1306_Command(0x12); // Alternative COM pin configuration
    
    SSD1306_Command(0xDB); // Set vcomh
    SSD1306_Command(0x20); // 0.77xVcc
    
    SSD1306_Command(0x8D); // Set DC-DC enable
    SSD1306_Command(0x14); // Enable charge pump
    
    SSD1306_Command(0xAF); // Display ON
}

// Очистка дисплея
void SSD1306_Clear(void) {
    uint8_t page, column;
    
    for (page = 0; page < 8; page++) {
        SSD1306_SetPosition(page, 0);
        for (column = 0; column < 128; column++) {
            SSD1306_Data(0x00); // Запись нулей
        }
    }
}

// Установка позиции курсора
void SSD1306_SetPosition(uint8_t page, uint8_t column) {
    SSD1306_Command(0xB0 + page);        // Set page address
    SSD1306_Command(0x00 + (column & 0x0F)); // Set lower column address
    SSD1306_Command(0x10 + ((column >> 4) & 0x0F)); // Set higher column address
}

// Отображение шахматной доски
void SSD1306_ChessBoard(void) {
    uint8_t page, column;
    uint8_t pattern;
    
    for (page = 0; page < 8; page++) {
        SSD1306_SetPosition(page, 0);
        for (column = 0; column < 128; column++) {
            // Создание шахматного паттерна
            pattern = ((page + column) % 2) ? 0xAA : 0x55;
            SSD1306_Data(pattern);
        }
    }
}

// Тестовое изображение с различными паттернами
void SSD1306_TestPattern(void) {
    uint8_t page, column;
    
    // Паттерн 1: Вертикальные полосы
    for (page = 0; page < 2; page++) {
        SSD1306_SetPosition(page, 0);
        for (column = 0; column < 128; column++) {
            SSD1306_Data((column % 8 < 4) ? 0xFF : 0x00);
        }
    }
    
    // Паттерн 2: Горизонтальные полосы
    for (page = 2; page < 4; page++) {
        SSD1306_SetPosition(page, 0);
        for (column = 0; column < 128; column++) {
            SSD1306_Data((page % 2) ? 0xFF : 0x00);
        }
    }
    
    // Паттерн 3: Шахматная доска
    for (page = 4; page < 6; page++) {
        SSD1306_SetPosition(page, 0);
        for (column = 0; column < 128; column++) {
            SSD1306_Data(((page + column) % 2) ? 0xAA : 0x55);
        }
    }
    
    // Паттерн 4: Точки
    for (page = 6; page < 8; page++) {
        SSD1306_SetPosition(page, 0);
        for (column = 0; column < 128; column++) {
            SSD1306_Data((column % 4 == 0) ? 0x81 : 0x00);
        }
    }
}

// Простая функция задержки
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 1000; j++) {
            asm("nop");
        }
    }
}

// Главная функция
void main(void) {
    // Настройка портов
    TRISA = 0x00;   // Порты A как выходы
    TRISB = 0x00;   // Порты B как выходы
    TRISC = 0x00;   // Порты C как выходы
    TRISD = 0x00;   // Порты D как выходы
    
    // Инициализация периферии
    SPI_Init();
    SSD1306_Init();
    
    // Очистка дисплея
    SSD1306_Clear();
    delay_ms(1000);
    
    while(1) {
        // Отображение шахматной доски
        SSD1306_ChessBoard();
        delay_ms(3000);
        
        // Очистка
        SSD1306_Clear();
        delay_ms(500);
        
        // Отображение тестового паттерна
        SSD1306_TestPattern();
        delay_ms(3000);
        
        // Очистка
        SSD1306_Clear();
        delay_ms(500);
    }
}
