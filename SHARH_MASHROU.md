# شرح مشروع Shape Collector Game

```
═══════════════════════════════════════════════════════════════
           شرح تعليمي كامل لمشروع لعبة الرسوميات
           Shape Collector - بلغة C++ ومكتبة FreeGLUT
═══════════════════════════════════════════════════════════════
```

---

# فهرس المحتويات

1. [مقدمة عن المشروع](#1-مقدمة-عن-المشروع)
2. [هندسة المشروع (Architecture)](#2-هندسة-المشروع-architecture)
3. [شرح game.h - ملف تعريفات اللعبة](#3-شرح-gameh)
4. [شرح game.cpp - ملف منطق اللعبة](#4-شرح-gamecpp)
5. [شرح renderer.h - ملف إعلانات الرسم](#5-شرح-rendererh)
6. [شرح renderer.cpp - ملف الرسم](#6-شرح-renderercpp)
7. [شرح input.h - ملف إعلانات الإدخال](#7-شرح-inputh)
8. [شرح input.cpp - ملف الإدخال](#8-شرح-inputcpp)
9. [شرح main.cpp - ملف البداية](#9-شرح-maincpp)
10. [تدفق البرنامج الكامل](#10-تدفق-البرنامج-الكامل)
11. [المفاهيم الأساسية](#11-المفاهيم-الأساسية)

---

## 1. مقدمة عن المشروع

### ما هي اللعبة؟

**Shape Collector** هي لعبة رسوميات ثنائية الأبعاد (2D) مكتوبة بلغة C++، تستخدم مكتبة **FreeGLUT** (وهي مكتبة مساعدة لـ OpenGL). فكرة اللعبة بسيطة:

- أنت تتحكم بمثلث أزرق في أسفل الشاشة.
- من الأعلى تسقط أشكال (دوائر ذهبية، مربعات سيان).
- مهمتك: تحريك المثلث لالتقاط أكبر عدد من الأشكال.
- كل شكل يُعطي نقاطاً: الدائرة = 10 نقاط، المربع = 5 نقاط.
- إذا فاتك شكل وسقط خارج الشاشة، تخسر حياة من أصل 3.
- عندما تخسر كل الأرواح، تنتهي اللعبة.

### ما هي المكتبات المستخدمة؟

| المكتبة | الاستخدام |
|---------|-----------|
| `<GL/freeglut.h>` | إنشاء النافذة، التعامل مع keyboard/mouse، الرسم |
| `<math.h>` | دوال رياضية: `cos()` و `sin()` لرسم الدائرة |
| `<stdio.h>` | دالة `sprintf()` لتحويل الأرقام إلى نصوص |
| `<stdlib.h>` | دالة `rand()` لتوليد أرقام عشوائية، `exit()` للخروج |
| `<time.h>` | دالة `time()` لبذر الأرقام العشوائية |

### لماذا FreeGLUT؟

FreeGLUT هي نسخة مجانية ومفتوحة المصدر من GLUT (OpenGL Utility Toolkit). توفر هذه المكتبة أدوات لـ:
- إنشاء نافذة للرسم
- التعامل مع أحداث keyboard و mouse
- إنشاء حلقة رسم مستمرة (Main Loop)

---

## 2. هندسة المشروع (Architecture)

### هيكل الملفات

```
my_project/
│
├── main.cpp          # نقطة بدء البرنامج ونظام GLUT
├── game.h            # تعريفات البيانات والثوابت
├── game.cpp          # منطق اللعبة (Game Logic)
├── renderer.h        # إعلانات دوال الرسم
├── renderer.cpp      # تنفيذ دوال الرسم (Rendering)
├── input.h           # إعلانات دوال الإدخال
└── input.cpp         # تنفيذ دوال الإدخال (Keyboard + Mouse)
```

### العلاقات بين الملفات

```
                    main.cpp
                   /    |    \
                  /     |     \
                 /      |      \
          game.h    renderer.h  input.h
             |           |         |
          game.cpp   renderer.cpp  input.cpp
```

- `main.cpp` يشمل (includes) كل ملفات الهيدر: `game.h` + `renderer.h` + `input.h`
- `renderer.h` يشمل `game.h` لأنه يحتاج إلى struct `Collectible`
- `input.cpp` يشمل `game.h` و `input.h`
- `game.cpp` يشمل `game.h`

### توزيع المسؤوليات (Separation of Concerns)

| الملف | مسؤوليته |
|-------|-----------|
| `main.cpp` | إعداد النافذة، GLUT callbacks، الحلقة الرئيسية |
| `game.h` | تعريف أنواع البيانات (Player, Collectible, GameState) والثوابت |
| `game.cpp` | منطق اللعبة: spawn, collision, update, state machine |
| `renderer.h` | إعلانات دوال الرسم |
| `renderer.cpp` | جميع عمليات الرسم (إعادة استخدام الدوال القديمة + دوال جديدة) |
| `input.h` | إعلانات دوال الإدخال |
| `input.cpp` | معالجة keyboard و mouse events |

### تدفق تشغيل البرنامج

```
1. main()
   │
   ├── 2. glutInit()          ← تهيئة GLUT
   ├── 3. init()              ← إعدادات OpenGL
   ├── 4. initGame()          ← إعداد حالة اللعبة
   ├── 5. glutDisplayFunc()   ← تسجيل callback الرسم
   ├── 6. glutIdleFunc()      ← تسجيل callback التحديث
   ├── 7. glutKeyboardFunc()  ← تسجيل keyboard
   ├── 8. glutSpecialFunc()   ← تسجيل special keys
   ├── 9. glutMouseFunc()     ← تسجيل الماوس
   └── 10. glutMainLoop()     ← الحلقة اللانهائية
             │
             └── loop:
                 ├── display() ← يرسم حسب حالة اللعبة
                 └── update()  ← يحدث منطق اللعبة إذا كانت PLAYING
```

---

## 3. شرح game.h

### ما هذا الملف؟

هذا ملف **Header (هيدر)**، وظيفته **تعريف** (declare) كل ما هو مشترك بين بقية الملفات:
- أنواع البيانات الجديدة (structs)
- الثوابت (constants)
- المتغيرات العامة (global variables)
- إعلانات الدوال (function declarations)

### السطر 1-2: Include Guard

```cpp
#ifndef GAME_H
#define GAME_H
```

**الشرح:** هذه تسمى **Include Guard** (حارس التضمين). عندما يشمل ملف `game.h` أكثر من مرة (مثلاً `renderer.h` يشمل `game.h` و `main.cpp` يشمل `renderer.h` و `game.h`)، يتأكد الحارس أن المحتوى يُدرج **مرة واحدة فقط**، مما يمنع أخطاء التكرار.

### السطر 10: Game State (حالة اللعبة)

```cpp
enum GameState { START_SCREEN, PLAYING, GAME_OVER };
```

**الشرح:** الـ **enum** (اختصار لـ enumeration) هو نوع بيانات يحدد مجموعة من الثوابت المسماة. هنا لدينا 3 حالات:
- `START_SCREEN` = 0: شاشة البداية (القائمة الرئيسية)
- `PLAYING` = 1: اللاعب يلعب
- `GAME_OVER` = 2: انتهت اللعبة

**لماذا؟** نستخدم enum بدلاً من أرقام عادية (0, 1, 2) لأن الأسماء أوضح للقارئ.

### السطر 13-18: Player Struct

```cpp
typedef struct {
    float x, y;       // Position in world coordinates (-1 to 1)
    float r, g, b;    // Current color
    int score;        // Collected points
    int lives;        // Remaining lives (starts at 3)
} Player;
```

**الشرح:** **Struct** هو نوع بيانات نُعرّفه بأنفسنا، يجمع متغيرات متعددة تحت اسم واحد. هنا يمثل `Player` اللاعب:
- `x, y`: موقع اللاعب في الفضاء (من -1 إلى 1، حيث 0 = منتصف الشاشة)
- `r, g, b`: ألوان اللاعب (Red, Green, Blue) - كل لون من 0.0 إلى 1.0
- `score`: النقاط التي جمعها
- `lives`: عدد الأرواح المتبقية

### السطر 21-27: Collectible Struct

```cpp
typedef struct {
    float x, y;       // Position
    float speed;      // Fall speed per frame
    int type;         // 0 = ball (10pts), 1 = polygon (5pts)
    int active;       // 1 = visible and on screen
    int points;       // Point value
} Collectible;
```

**الشرح:** يمثل الشكل المتساقط الذي يجب على اللاعب التقاطه:
- `x, y`: موقعه في الشاشة (يبدأ من أعلى ويهبط لأسفل)
- `speed`: سرعة السقوط (كل شكل له سرعة مختلفة)
- `type`: 0 = دائرة (كرة)، 1 = مضلع (مربع)
- `active`: 1 إذا كان الشكل مرئياً ويتحرك، 0 إذا اختفى
- `points`: كم نقطة يعطي عند التقاطه

### السطر 30-33: الثوابت (Constants)

```cpp
#define MAX_ITEMS   10      // أقصى عدد للأشكال المتساقطة
#define BOUNDARY    0.85f   // حدود الحركة (يمين/يسار/أعلى/أسفل)
#define PLAYER_STEP 0.04f   // مقدار حركة اللاعب كل ضغطة
#define SPAWN_RATE  100     // عدد الفريمات بين كل spawn وآخر
```

**الشرح:** `#define` هو توجيه (directive) للمُصرِّف (compiler)، يستبدل النص قبل التجميع:
- `MAX_ITEMS = 10`: لا يمكن أن يكون أكثر من 10 أشكال على الشاشة
- `BOUNDARY = 0.85f`: اللاعب لا يستطيع تخطي 85% من الشاشة (هامش 15% للحواف)
- `PLAYER_STEP = 0.04f`: كل ضغطة سهم حركة = 4% من عرض الشاشة
- `SPAWN_RATE = 100`: شكل جديد كل 100 فريم (حوالي 1.6 ثانية عند 60 فريم/ثانية)

**ملاحظة:** `f` بعد الرقم تعني Float (عدد عشري بنصف دقة).

### السطر 36-39: المتغيرات العامة (Global Variables)

```cpp
extern GameState gameState;
extern Player player;
extern Collectible items[MAX_ITEMS];
extern int spawnTimer;
```

**الشرح:** كلمة `extern` تعني "هذا المتغير موجود في ملف آخر". التهيئة الفعلية (definition) موجودة في `game.cpp`:
- `gameState`: حالة اللعبة الحالية
- `player`: بيانات اللاعب
- `items[10]`: مصفوفة من 10 أشكال متساقطة
- `spawnTimer`: عداد الفريمات لظهور شكل جديد

### السطر 42-46: إعلانات الدوال (Function Declarations)

```cpp
void initGame();
void resetGame();
void updateGame();
void spawnItem();
void checkCollisions();
```

**الشرح:** هذه إعلانات فقط (prototypes) - التنفيذ الفعلي في `game.cpp`:
- `initGame()`: تجهيز اللعبة من البداية
- `resetGame()`: إعادة تعيين للعبة جديدة دون تغيير الحالة
- `updateGame()`: تحديث اللعبة كل فريم
- `spawnItem()`: إنشاء شكل جديد
- `checkCollisions()`: فحص التصادمات

---

## 4. شرح game.cpp

### السطر 12-16: تعريف المتغيرات العامة

```cpp
GameState gameState = START_SCREEN;
Player player;
Collectible items[MAX_ITEMS];
int spawnTimer = 0;
```

**الشرح:** هنا يتم **التعريف الفعلي** (definition) للمتغيرات التي أعلنها `game.h` بكلمة `extern`. تبدأ اللعبة بحالة `START_SCREEN`.

### دالة initGame() - الأسطر 19-32

```cpp
void initGame() {
    player.x = 0.0f;
    player.y = -0.7f;
    player.r = 0.2f; player.g = 0.6f; player.b = 1.0f;
    player.score = 0;
    player.lives = 3;
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].active = 0;
    }
    spawnTimer = 0;
    gameState = START_SCREEN;
}
```

**الوظيفة:** تهيئة اللعبة من البداية.

**تحليل السطور:**
- `player.x = 0.0f; player.y = -0.7f;`: اللاعب يبدأ في منتصف الشاشة أفقياً (x=0) وأسفل الشاشة (y=-0.7)
- `player.r = 0.2f; ... = 1.0f;`: اللون أزرق فاتح (0.2 أحمر، 0.6 أخضر، 1.0 أزرق)
- `player.score = 0;`: النقاط = 0
- `player.lives = 3;`: 3 أرواح
- الحلقة: تجعل كل الـ items غير نشطة (`active = 0`)
- `spawnTimer = 0;`: تصفير عداد الظهور
- `gameState = START_SCREEN;`: الحالة = شاشة البداية

**متى تُستدعى؟** مرة واحدة فقط في `main()` عند بداية البرنامج.

### دالة resetGame() - الأسطر 35-46

```cpp
void resetGame() {
    player.x = 0.0f;
    player.y = -0.7f;
    player.score = 0;
    player.lives = 3;
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].active = 0;
    }
    spawnTimer = 0;
}
```

**الوظيفة:** إعادة اللعبة للحالة الأولية (بدون تغيير `gameState` إلى START_SCREEN).

**الفرق عن initGame():**
- `initGame()` تستدعى **مرة واحدة** عند بداية البرنامج وتضع الحالة START_SCREEN.
- `resetGame()` تستدعى عندما يضغط اللاعب SPACE بعد GAME_OVER وتضع الحالة PLAYING.

**متى تُستدعى؟** في `input.cpp` عندما يضغط اللاعب SPACE في حالة GAME_OVER.

### دالة spawnItem() - الأسطر 50-62

```cpp
void spawnItem() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) {
            items[i].x = ((rand() % 17) - 8) / 10.0f;
            items[i].y = 0.95f;
            items[i].speed = 0.004f + (rand() % 6) * 0.002f;
            items[i].type = rand() % 2;
            items[i].active = 1;
            items[i].points = (items[i].type == 0) ? 10 : 5;
            break;
        }
    }
}
```

**الوظيفة:** إنشاء شكل متساقط جديد في موقع عشوائي.

**تحليل السطور خطوة بخطوة:**
1. `for (int i = 0; i < MAX_ITEMS; i++)`: نبحث في مصفوفة الـ 10 items عن أول عنصر غير نشط (active = 0)
2. `if (!items[i].active)`: ! تعني NOT، أي إذا كان غير نشط، نستخدمه
3. حساب الموقع X العشوائي:
   - `rand() % 17`: رقم عشوائي من 0 إلى 16
   - `-8`: يصبح من -8 إلى 8
   - `/ 10.0f`: يصبح من -0.8 إلى 0.8
4. `items[i].y = 0.95f`: يبدأ من أعلى الشاشة (قريب من الحافة العلوية)
5. `rand() % 6`: رقم من 0 إلى 5، نضربه بـ 0.002f ونضيف 0.004f نحصل على سرعة بين 0.004 و 0.014
6. `rand() % 2`: 0 أو 1 (نوع الشكل)
7. `active = 1`: ننشطه
8. `?:` (Ternary Operator): إذا type=0 → 10 نقاط، وإلا → 5 نقاط
9. `break;`: نخرج من الحلقة بعد أن وجدنا عنصراً

**مثال على rand():**
```
إذا rand() أعطت 12:
  x = (12 % 17 = 12) → 12 - 8 = 4 → 4 / 10 = 0.4
  أي يظهر في منتصف الشاشة جهة اليمين
```

### دالة updateGame() - الأسطر 66-92

```cpp
void updateGame() {
    spawnTimer++;
    if (spawnTimer >= SPAWN_RATE) {
        spawnItem();
        spawnTimer = 0;
    }
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active) {
            items[i].y -= items[i].speed;
            if (items[i].y < -1.0f) {
                items[i].active = 0;
                player.lives--;
                if (player.lives <= 0) {
                    gameState = GAME_OVER;
                }
            }
        }
    }
    checkCollisions();
}
```

**الوظيفة:** تحديث اللعبة كل فريم - تُستدعى من `update()` في `main.cpp`.

**تحليل منطق spawn:**
- `spawnTimer++`: نزيد العداد بواحد كل فريم
- إذا وصل العداد 100 (SPAWN_RATE)، نستدعي `spawnItem()` ونصفّر العداد
- هذا يضمن ظهور شكل جديد كل 100 فريم تقريباً

**تحليل حركة السقوط:**
- لكل item نشط: نطرح السرعة من Y (نحركه لأسفل)
- إذا أصبح Y أقل من -1 (خرج من الشاشة): نلغيه ونخسر حياة
- إذا وصلت الأرواح لـ 0: gameState = GAME_OVER

### دالة checkCollisions() - الأسطر 96-109

```cpp
void checkCollisions() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active) {
            float dx = player.x - items[i].x;
            float dy = player.y - items[i].y;
            float distSq = dx * dx + dy * dy;
            if (distSq < 0.032f) {
                items[i].active = 0;
                player.score += items[i].points;
            }
        }
    }
}
```

**الوظيفة:** فحص إذا كان اللاعب قد التقط شكلاً.

**تحليل طريقة كشف التصادم (Collision Detection):**
1. نحسب الفرق بين موقع اللاعب وموقع الـ item في X و Y
2. `distSq = dx² + dy²`: مربع المسافة (نستخدم مربع المسافة لتجنب حساب الجذر التربيعي المكلف)
3. إذا كانت أقل من 0.032، يعني أن المسافة الفعلية أقل من 0.178 (الجذر التربيعي لـ 0.032):
   - `sqrt(0.032) ≈ 0.178`
   - هذا هو نصف قطر التصادم (Collision Radius)
4. عند التصادم: نلغي الـ item و نضيف نقاطه للاعب

**لماذا distSq بدلاً من dist؟**
لو استخدمنا المسافة الحقيقية، كنا سنحتاج:
```cpp
float dist = sqrt(dx*dx + dy*dy);
if (dist < 0.178f) ...
```
لكن دالة `sqrt()` بطيئة (تستغرق وقتاً في الحساب)، لذلك نستخدم `distSq` مباشرة ونقارنها بـ `0.032` (وهو `0.178²`).

---

## 5. شرح renderer.h

**الوظيفة:** هذا ملف هيدر يعلن عن دوال الرسم، ليتمكن `main.cpp` من استخدامها.

```cpp
#include "game.h"   // يحتاج إلى تعريف Collectible
```

**لماذا يشمل game.h؟** لأن الدالة `drawCollectible()` تستقبل `Collectible*` كمُدخل، والمُصرِّف يحتاج لرؤية تعريف هذا النوع.

---

## 6. شرح renderer.cpp

### دوال الرسم المُعاد استخدامها (Reused Functions)

#### drawTriangle() - الأسطر 22-28

```cpp
void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 0.3);
    glVertex2f(-0.3, -0.3);
    glVertex2f(0.3, -0.3);
    glEnd();
}
```

**الوظيفة:** رسم مثلث في منتصف الشاشة.

**تحليل OpenGL:**
- `glBegin(GL_TRIANGLES)`: نقول لـ OpenGL أننا سنرسم مثلثاً. كل 3 نقاط تشكل مثلثاً واحداً.
- `glVertex2f(x, y)`: نحدد نقطة (vertex). المعاملات: 2f = موقع ثنائي الأبعاد (float).
- `glEnd()`: ننهي الرسم.

الرءوس (vertices):
- `(0.0, 0.3)`: القمة (أعلى)
- `(-0.3, -0.3)`: الزاوية السفلى اليسرى
- `(0.3, -0.3)`: الزاوية السفلى اليمنى

الإحداثيات:
```
        (0.0, 0.3)
          /\
         /  \
        /    \
       /______\
(-0.3, -0.3)  (0.3, -0.3)
```

#### drawBall() - الأسطر 32-39

```cpp
void drawBall() {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float angle = i * 2.0f * 3.1416f / 100;
        glVertex2f(0.1f * cos(angle), 0.1f * sin(angle));
    }
    glEnd();
}
```

**الوظيفة:** رسم دائرة (كرة).

**المنطق:** يرسم مضلعاً بـ 100 ضلع. كلما زاد عدد الأضلاع، أصبح الشكل أقرب للدائرة.

**معادلة الدائرة:**
- أي نقطة على دائرة نصف قطرها r وزاويتها θ:
  - `x = r × cos(θ)`
  - `y = r × sin(θ)`

**الحلقة:** نولد 100 نقطة:
- `i = 0` → angle = 0 → `(0.1, 0)` (يمين الدائرة)
- `i = 25` → angle = π/2 → `(0, 0.1)` (أعلى الدائرة)
- `i = 50` → angle = π → `(-0.1, 0)` (يسار الدائرة)
- `i = 75` → angle = 3π/2 → `(0, -0.1)` (أسفل الدائرة)

**القيم:** نصف القطر = 0.1، عدد النقاط = 100.

#### drawPolygon() - الأسطر 43-51

```cpp
void drawPolygon() {
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.9, 0.9f);
    glVertex2f(0.1f, 0.1f);
    glVertex2f(-0.1f, 0.1f);
    glVertex2f(-0.1f, -0.1f);
    glVertex2f(0.1f, -0.1f);
    glEnd();
}
```

**الوظيفة:** رسم مربع صغير (مضلع بأربعة أضلاع).

نقطة البداية (0.1, 0.1) = الزاوية اليمنى العليا، ثم ننتقل عكس عقارب الساعة.

### دوال الرسم الجديدة

#### renderText() - الأسطر 58-63

```cpp
void renderText(float x, float y, void* font, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
```

**المدخلات:**
- `x, y`: الموقع في الشاشة
- `font`: نوع الخط (مثلاً `GLUT_BITMAP_HELVETICA_18`)
- `text`: النص الذي نريد رسمه

**الشرح:**
- `glRasterPos2f(x, y)`: نحدد أين يبدأ النص
- `for (const char* c = text; *c != '\0'; c++)`: الحلقة تمر على كل حرف في النص
  - تبدأ من أول حرف
  - تستمر حتى تصل إلى `\0` (نهاية النص)
  - `c++` تنتقل للحرف التالي
- `glutBitmapCharacter(font, *c)`: ترسم حرفاً واحداً باستخدام FreeGLUT

**مثال:** إذا استدعينا `renderText(0, 0, font, "Hi")`:
1. ترسم 'H' في (0, 0)
2. ترسم 'i' بعد 'H' مباشرة

#### drawPlayer() - الأسطر 66-73

```cpp
void drawPlayer() {
    glPushMatrix();
    glTranslatef(player.x, player.y, 0.0f);
    glScalef(0.5f, 0.5f, 1.0f);
    glColor3f(player.r, player.g, player.b);
    drawTriangle();
    glPopMatrix();
}
```

**الوظيفة:** رسم اللاعب (مثلث ملون) في موقعه الحالي.

**تحليل المصفوفات (Matrix Stack):**
- `glPushMatrix()`: يحفظ المصفوفة الحالية (الموقع والقياس والدوران) في كومة (Stack)
- `glTranslatef(x, y, z)`: يحرك نقطة الأصل (origin) إلى موقع اللاعب
- `glScalef(sx, sy, sz)`: يصغر المثلث إلى نصف حجمه (0.5)
- `glColor3f(r, g, b)`: يضبط اللون حسب قيم اللاعب
- `drawTriangle()`: يرسم المثلث (في الموقع الجديد بعد الترجمة والقياس)
- `glPopMatrix()`: يستعيد المصفوفة القديمة

**لماذا Push/Pop؟** حتى لا تؤثر الترجمة والقياس على الرسومات الأخرى. نفصل كل رسم في "عالمه الخاص".

#### drawCollectible() - الأسطر 76-89

```cpp
void drawCollectible(Collectible* item) {
    glPushMatrix();
    glTranslatef(item->x, item->y, 0.0f);
    if (item->type == 0) {
        glColor3f(1.0f, 0.84f, 0.0f);   // ذهبي
        drawBall();
    } else {
        glColor3f(0.0f, 0.9f, 0.9f);     // سيان
        drawPolygon();
    }
    glPopMatrix();
}
```

**الوظيفة:** رسم شكل متساقط في موقعه.

**المدخل:** مؤشر (pointer) إلى `Collectible` - نستخدم `->` للوصول إلى أعضاء الـ struct.

**المنطق:**
- إذا type=0: لون ذهبي + `drawBall()`
- إذا type=1: لون سيان + `drawPolygon()`

#### drawHUD() - الأسطر 105-122

```cpp
void drawHUD() {
    char buffer[32];
    sprintf(buffer, "Score: %d", player.score);
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(-0.18f, 0.92f, GLUT_BITMAP_HELVETICA_18, buffer);
    for (int i = 0; i < player.lives; i++) {
        glPushMatrix();
        glTranslatef(-0.88f + i * 0.1f, 0.92f, 0.0f);
        glScalef(0.3f, 0.3f, 1.0f);
        glColor3f(1.0f, 0.2f, 0.2f);
        drawTriangle();
        glPopMatrix();
    }
}
```

**الوظيفة:** عرض النقاط والأرواح في أعلى الشاشة.

**النقاط:** `sprintf(buffer, "Score: %d", player.score)` تحول الرقم إلى نص (تنسيق C-style).
- `%d` = مكان الرقم العشري
- إذا score=25 → النص "Score: 25"

**الأرواح:** حلقة ترسم 3 مثلثات حمراء صغيرة بجانب بعضها:
- i=0 → x = -0.88
- i=1 → x = -0.78
- i=2 → x = -0.68

#### drawStartScreen() - الأسطر 125-157

```cpp
void drawStartScreen() {
    glColor3f(1.0f, 0.85f, 0.0f);
    renderText(-0.45f, 0.65f, GLUT_BITMAP_TIMES_ROMAN_24, "SHAPE COLLECTOR");
    // ... المزيد من النصوص
    // أشكال زخرفية
    glPushMatrix();
    glTranslatef(-0.75f, 0.75f, 0.0f);
    glColor3f(1.0f, 0.85f, 0.0f);
    drawBall();
    glPopMatrix();
    // ...
}
```

**الوظيفة:** عرض شاشة البداية مع عنوان اللعبة وتعليمات التشغيل.

تستخدم `renderText()` لعرض 5 أسطر من النصوص، ثم ترسم 3 أشكال زخرفية (كرة، مضلع، مثلث) حول الشاشة.

#### drawGameOverScreen() - الأسطر 160-172

**الوظيفة:** عرض شاشة نهاية اللعبة مع النتيجة النهائية.

#### drawGameScene() - الأسطر 176-197

```cpp
void drawGameScene() {
    switch (gameState) {
        case START_SCREEN: drawStartScreen(); break;
        case PLAYING:
            drawBorder();
            drawHUD();
            drawPlayer();
            for (int i = 0; i < MAX_ITEMS; i++) {
                if (items[i].active) {
                    drawCollectible(&items[i]);
                }
            }
            break;
        case GAME_OVER: drawGameOverScreen(); break;
    }
}
```

**الوظيفة:** المُوزِّع الرئيسي للرسم. بناءً على `gameState`، تستدعي الدالة المناسبة.

**لماذا switch؟** لأنه أوضح من if-else المتكرر، ويسمح بإضافة حالات جديدة بسهولة.

---

## 7. شرح input.h

```cpp
void handleFreeKeys(unsigned char key, int mx, int my);
void handleSpecialKeys(int key, int mx, int my);
void mouseHandler(int button, int state, int mx, int my);
```

هذه إعلانات لدوال الإدخال الثلاث. المعاملات (parameters) تأتي تلقائياً من FreeGLUT عند وقوع الحدث.

---

## 8. شرح input.cpp

### handleFreeKeys() - الأسطر 17-55

```cpp
void handleFreeKeys(unsigned char key, int mx, int my) {
    switch (key) {
        case ' ':
            if (gameState == START_SCREEN)
                gameState = PLAYING;
            else if (gameState == GAME_OVER) {
                resetGame();
                gameState = PLAYING;
            }
            break;
        case 'a': case 'A':
            if (gameState == PLAYING && player.x > -BOUNDARY)
                player.x -= PLAYER_STEP;
            break;
        // ... WASD + ESC
    }
    glutPostRedisplay();
}
```

**المدخلات:**
- `key`: الحرف الذي ضغطه المستخدم (char)
- `mx, my`: موقع الماوس (لا نستخدمه هنا، لكن GLUT يرسله)

**الوظيفة:** تتعامل مع ضغطات لوحة المفاتيح العادية.

**تحليل المنطق:**
1. **Space (رمز ' ')**:
   - إذا كنا في START_SCREEN → نبدأ اللعب
   - إذا كنا في GAME_OVER → نعيد التعيين ونبدأ لعباً جديداً
2. **WASD (a, d, w, s)**: نتحقق أولاً أننا في حالة PLAYING وأن اللاعب لن يتجاوز الحدود (BOUNDARY)، ثم نحرك
3. **ESC (رقم 27)**: `exit(0)` ينهي البرنامج فوراً

**لماذا `case 'a': case 'A':`؟** لمعالجة الأحرف الكبيرة والصغيرة معاً.

**لماذا glutPostRedisplay() في النهاية؟** لإخبار GLUT بإعادة رسم الشاشة بعد تغيير الموقع.

### handleSpecialKeys() - الأسطر 59-80

```cpp
void handleSpecialKeys(int key, int mx, int my) {
    switch (key) {
        case GLUT_KEY_LEFT:  // ... تحريك لليسار
        case GLUT_KEY_RIGHT: // ... تحريك لليمين
        case GLUT_KEY_UP:    // ... تحريك لأعلى
        case GLUT_KEY_DOWN:  // ... تحريك لأسفل
    }
    glutPostRedisplay();
}
```

**الوظيفة:** معالجة مفاتيح الأسهم (Arrow Keys).

**الفرق:** FreeGLUT يعتبر الأسهم "special keys" ويحتاج دالة منفصلة (`glutSpecialFunc`). `GLUT_KEY_LEFT` إلخ هي ثوابت معرّفة في FreeGLUT.

### mouseHandler() - الأسطر 84-95

```cpp
void mouseHandler(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (gameState == PLAYING) {
            player.r = 0.3f + (rand() % 70) / 100.0f;
            player.g = 0.3f + (rand() % 70) / 100.0f;
            player.b = 0.3f + (rand() % 70) / 100.0f;
        }
    }
    glutPostRedisplay();
}
```

**المدخلات:**
- `button`: الزر الذي ضغط: `GLUT_LEFT_BUTTON`, `GLUT_RIGHT_BUTTON`, `GLUT_MIDDLE_BUTTON`
- `state`: `GLUT_DOWN` (ضغط) أو `GLUT_UP` (رفع)
- `mx, my`: إحداثيات الماوس (بالـ pixels)

**الوظيفة:** تغيير لون اللاعب عشوائياً عند الضغط على زر الفأرة الأيسر.

**تحليل توليد اللون العشوائي:**
```cpp
player.r = 0.3f + (rand() % 70) / 100.0f;
```
- `rand() % 70`: رقم من 0 إلى 69
- `/ 100.0f`: يصبح من 0.00 إلى 0.69
- `+ 0.3f`: يصبح من 0.30 إلى 0.99 (ألوان زاهية، ليست داكنة جداً)

---

## 9. شرح main.cpp

### الأسطر 17-22: التضمينات (Includes)

```cpp
#include <GL/freeglut.h>   // مكتبة الرسوميات
#include <stdlib.h>         // rand(), srand(), exit()
#include <time.h>           // time()
#include "game.h"           // تعريفات اللعبة
#include "renderer.h"       // دوال الرسم
#include "input.h"          // دوال الإدخال
```

الترتيب مهم: المكتبات العامة أولاً (بأقواس `<>`)، ثم ملفات المشروع (بعلامات `""`).

### دالة init() - الأسطر 26-33

```cpp
void init() {
    glClearColor(0.05f, 0.05f, 0.12f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}
```

**الوظيفة:** إعدادات OpenGL الأساسية.

**تحليل كل سطر:**
1. `glClearColor(0.05, 0.05, 0.12, 1.0)`:
   - لون الخلفية عند مسح الشاشة
   - R=0.05, G=0.05, B=0.12 (أزرق داكن جداً)
   - Alpha=1.0 (غير شفاف)
   - هذه القيم ستُستخدم عند استدعاء `glClear()`

2. `glMatrixMode(GL_PROJECTION)`:
   - نختار مصفوفة الإسقاط (Projection Matrix)
   - الإسقاط يحدد كيف تُعرض المشاهد ثلاثية الأبعاد على الشاشة ثنائية الأبعاد

3. `glLoadIdentity()`:
   - نحمّل مصفوفة الوحدة (Identity Matrix) - أي إعادة تعيين
   - هذا يمسح أي تحويلات سابقة

4. `gluOrtho2D(-1, 1, -1, 1)`:
   - نحدد نظام الإحداثيات ثنائي الأبعاد
   - اليسار = -1، اليمين = 1، الأسفل = -1، الأعلى = 1
   - هذا يعني أن (0, 0) هو منتصف الشاشة
   - والعرض الكامل من -1 إلى 1 في كلا الاتجاهين

5. `glMatrixMode(GL_MODELVIEW)`:
   - نعود لمصفوفة النموذج/العرض (ModelView Matrix)
   - هذه المصفوفة تتحكم بموقع الأشكال

### دالة display() - الأسطر 37-44

```cpp
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawGameScene();
    glutSwapBuffers();
}
```

**الوظيفة:** مسح الشاشة ورسم المشهد الحالي.

- `glClear(GL_COLOR_BUFFER_BIT)`: تمسح الشاشة بلون الخلفية (الذي حددناه في init)
- `glLoadIdentity()`: تعيد تعيين المصفوفات (بداية نظيفة)
- `drawGameScene()`: تستدعي الرسم المناسب حسب حالة اللعبة
- `glutSwapBuffers()`: تبدّل المخازن (buffers) - لأننا نستخدم `GLUT_DOUBLE`

**لماذا GLUT_DOUBLE؟** 
- **Single buffer:** ترسم مباشرة على الشاشة (يسبب وميضاً flicker)
- **Double buffer:** ترسم في مخبأ خلفي (back buffer) ثم تظهره دفعة واحدة (أملس)

### دالة update() - الأسطر 49-54

```cpp
void update() {
    if (gameState == PLAYING) {
        updateGame();
    }
    glutPostRedisplay();
}
```

**الوظيفة:** تحديث اللعبة باستمرار (Idle callback).

- كلما كان GLUT خاملاً (لا يعالج أحداثاً أخرى)، يستدعي هذه الدالة
- إذا كنا نلعب (`PLAYING`)، نحدّث اللعبة
- `glutPostRedisplay()`: يطلب من GLUT إعادة رسم الشاشة (استدعاء display)

### دالة main() - الأسطر 58-80

```cpp
int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));              // بذر العشوائية

    glutInit(&argc, argv);                        // تهيئة GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // وضع العرض
    glutInitWindowSize(800, 600);                 // حجم النافذة
    glutInitWindowPosition(120, 80);              // موقع النافذة
    glutCreateWindow("Shape Collector ...");      // إنشاء النافذة

    init();               // إعدادات OpenGL
    initGame();           // تهيئة اللعبة

    // تسجيل callbacks:
    glutDisplayFunc(display);      // الرسم
    glutIdleFunc(update);          // التحديث المستمر
    glutKeyboardFunc(handleFreeKeys);  // لوحة المفاتيح
    glutSpecialFunc(handleSpecialKeys); // الأسهم
    glutMouseFunc(mouseHandler);    // الفأرة

    glutMainLoop();        // الحلقة الرئيسية (لا تعود أبداً)
    return 0;
}
```

**تحليل glutInitDisplayMode:**
- `GLUT_DOUBLE`: مخزنان (double buffering) - لمنع الوميض
- `GLUT_RGB`: نظام ألوان RGB (أحمر-أخضر-أزرق)

**ما هي الـ Callbacks؟**
الدوال التي تسجلها (`glutDisplayFunc`, `glutKeyboardFunc`...) تسمى Callbacks. بدلاً من أن تستدعيها أنت، GLUT هو من يستدعيها عند حدوث الحدث المناسب.

**ما هو glutMainLoop؟**
هي الحلقة اللانهائية التي تدير اللعبة:
```
while (true) {
    معالجة الأحداث (keyboard, mouse, etc.)
    استدعاء update() ← تحديث المنطق
    استدعاء display() ← رسم المشهد
}
```
هذه الدالة لا تعود أبداً (`return 0` لن يُنفذ).

---

## 10. تدفق البرنامج الكامل

### السيناريو الكامل من البداية للنهاية:

```
1. تشغيل البرنامج
   ↓
2. main() يستدعي:
   - glutInit() ← تهيئة
   - init() ← خلفية زرقاء + نظام إحداثيات
   - initGame() ← لاعب في منتصف القاع، 3 أرواح، score=0
   - glutMainLoop() ← بداية الحلقة
   ↓
3. glutMainLoop() يستدعي display()
   ↓
4. display() → drawGameScene()
   ↓
5. drawGameScene(): gameState = START_SCREEN
   → drawStartScreen()
     - يعرض "SHAPE COLLECTOR" بالذهب
     - يعرض التعليمات
     - يرسم أشكالاً زخرفية
   ↓
6. ينتظر GLUT أحداث المستخدم
   ↓
7. المستخدم يضغط SPACE
   ↓
8. handleFreeKeys(' ', ...) تكتشف Space
   → gameState = PLAYING
   ↓
9. display() → drawGameScene() → PLAYING
   → drawBorder() + drawHUD() + drawPlayer()
   ↓
10. update() → updateGame() كل فريم:
    - spawnTimer يزيد
    - عندما spawnTimer = 100 → spawnItem()
      → شكل جديد يسقط من أعلى
    - كل item ينزل بسرعة items[i].speed
    - checkCollisions() تفحص إذا التقط اللاعب شكلاً
    ↓
11. عندما يلتقط اللاعب شكلاً:
    → score يزيد
    → الشكل يختفي
    ↓
12. عندما يفوت اللاعب شكلاً:
    → حياة تنقص
    → عندما lives = 0 → gameState = GAME_OVER
    ↓
13. display() → drawGameScene() → GAME_OVER
    → drawGameOverScreen()
    ↓
14. المستخدم يضغط SPACE
    → resetGame() + gameState = PLAYING
    → لعبة جديدة
    ↓
15. المستخدم يضغط ESC
    → exit(0) → البرنامج يغلق
```

---

## 11. المفاهيم الأساسية

### 1. Event Handling (معالجة الأحداث)

**ما هو؟** نظام يستجيب لإجراءات المستخدم (ضغط زر، حركة ماوس) بدلاً من تنفيذ أوامر ثابتة.

**في مشروعنا:** FreeGLUT يوفر نظام الأحداث. نسجل دالة معينة لكل حدث:
- `glutKeyboardFunc(handleFreeKeys)`: عند ضغط حرف
- `glutSpecialFunc(handleSpecialKeys)`: عند ضغط سهم
- `glutMouseFunc(mouseHandler)`: عند ضغط زر الفأرة

عند حدوث الحدث، GLUT يستدعي الدالة المسجلة تلقائياً مع المعلومات المناسبة.

### 2. Game Loop (حلقة اللعبة)

**ما هي؟** دورة مستمرة تتكرر طوال تشغيل اللعبة:
```
معالجة الإدخال ← تحديث المنطق ← الرسم
```

**في مشروعنا:**
```
glutMainLoop() تكرر:
  glutKeyboardFunc → handleFreeKeys (أحداث keyboard)
  glutIdleFunc     → update() (تحديث اللعبة)
  glutDisplayFunc  → display() (رسم المشهد)
```

### 3. Rendering Pipeline (خط أنابيب الرسم)

**ما هو؟** سلسلة العمليات التي تحول الأوامر الرسومية إلى بكسلات على الشاشة.

**في مشروعنا:**
```
1. glClear() ← مسح الشاشة
2. glLoadIdentity() ← إعادة تعيين المصفوفات
3. glPushMatrix() ← حفظ الحالة الحالية
4. glTranslatef()/glScalef() ← تطبيق التحويلات
5. glBegin/glVertex/glEnd ← إرسال الرءوس
6. glPopMatrix() ← استعادة الحالة
```

### 4. Coordinate System (نظام الإحداثيات)

**ما هو؟** يحدد كيفية تفسير الأرقام كمواقع على الشاشة.

**في مشروعنا:**
- `gluOrtho2D(-1, 1, -1, 1)`:
  - (0, 0) = وسط الشاشة
  - (-1, -1) = أسفل يسار
  - (1, 1) = أعلى يمين
  - (0.5, 0) = يمين المنتصف بقليل

### 5. Matrix Stack (كومة المصفوفات)

**ما هو؟** تقنية لحفظ واستعادة تحويلات الرسم (ترجمة، تدوير، تحجيم).

**في مشروعنا:**
```cpp
glPushMatrix();    // احفظ الحالة الحالية
  glTranslatef(x, y, 0);  // حرّك نقطة الأصل
  glScalef(0.5, 0.5, 1);  // صغّر الحجم
  drawTriangle();          // ارسم في الموقع الجديد
glPopMatrix();     // استعد الحالة القديمة
```

**لماذا؟** حتى لا تؤثر التحويلات على الرسومات الأخرى. كل رسم في "عالمه الخاص".

### 6. Collision Detection (كشف التصادمات)

**ما هو؟** تحديد عندما يتقاطع جسمان في الفضاء.

**طريقتنا - Circle-Circle Collision (دائرة مع دائرة):**
- نعتبر كل شكل له دائرة وهمية حوله
- نحسب المسافة بين مركزَي الدائرتين
- إذا كانت المسافة أقل من مجموع نصفي القطرين → تصادم

**في كودنا:**
```cpp
dx = player.x - items[i].x;
dy = player.y - items[i].y;
distSq = dx*dx + dy*dy;
if (distSq < 0.032f)  // sqrt(0.032) ≈ 0.178
    collision!
```

### 7. Delta Time / Frame-based Timing (التوقيت بالفريمات)

**ما هو؟** بدلاً من استخدام الوقت الحقيقي (بالثواني)، نستخدم عدد الفريمات.

**في مشروعنا:**
```cpp
spawnTimer++;          // كل فريم يزيد العداد
if (spawnTimer >= 100) // بعد 100 فريم
    spawnItem();
```

بمعدل 60 فريم/ثانية، 100 فريم = حوالي 1.6 ثانية.

### 8. State Machine (آلة الحالات)

**ما هو؟** نظام يتغير بين حالات محددة مسبقاً حسب المدخلات والأحداث.

**في مشروعنا:**
```
           SPACE
START_SCREEN ──────→ PLAYING
                        │
                   lives = 0
                        │
                        ↓
                   GAME_OVER
                        │
                   SPACE + resetGame()
                        ↓
                   PLAYING
```

### 9. Double Buffering (المخازن المزدوجة)

**ما هو؟** استخدام مخزنين للرسم: أمامي (يعرض) وخلفي (يرسم). عندما يكتمل الرسم في الخلفي، نبدلهما.

**فائدته:** يمنع الوميض (flickering) الذي يحدث عندما ترسم مباشرة على الشاشة والشاشة تحدث نفسها في منتصف الرسم.

**في كودنا:**
```cpp
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
// ...
glutSwapBuffers();  // تبديل المخازن
```

---

# خلاصة

مشروع **Shape Collector** هو تطبيق عملي لمفاهيم:

| المفهوم | أين تجده في الكود |
|---------|-------------------|
| الرسوميات (OpenGL) | `renderer.cpp` - glBegin, glVertex, glColor |
| معالجة الأحداث | `input.cpp` - keyboard/mouse callbacks |
| حلقة اللعبة | `glutMainLoop()` + `update()` + `display()` |
| آلة الحالات | `gameState` في `game.h` و `renderer.cpp` |
| كشف التصادم | `checkCollisions()` في `game.cpp` |
| المصفوفات والتحويلات | `glPushMatrix/PopMatrix/Translatef/Scalef` |
| Double Buffering | `GLUT_DOUBLE` + `glutSwapBuffers()` |
| النظام الإحداثي | `gluOrtho2D(-1, 1, -1, 1)` |
| إعادة استخدام الكود | `drawTriangle(), drawBall(), drawPolygon()` من المشاريع السابقة |

---

*تم إعداد هذا الشرح التعليمي لتوثيق وفهم مشروع Shape Collector خطوة بخطوة.*
