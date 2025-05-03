from ursina import *
from random import randint

app = Ursina()
camera.orthographic = True
camera.fov = 20

grid_size = 20
snake = []
direction = Vec2(1, 0)
food = None
timer = 0
playing = False
menu_ui = None
game_over_ui = None
score = 0  # Skor değişkenini tanımladık

def add_tail():
    last = snake[-1]
    new = Entity(model='quad', color=color.lime, scale=0.9, position=last.position)
    snake.append(new)

def move_food():
    food.position = (randint(-grid_size//2, grid_size//2),
                     randint(-grid_size//2, grid_size//2), 0)

def start_game():
    global snake, direction, food, timer, playing, score

    for s in snake:
        destroy(s)
    if food:
        destroy(food)

    snake = [Entity(model='quad', color=color.green, scale=0.9, position=(0,0))]
    direction = Vec2(1, 0)
    food = Entity(model='quad', color=color.red, scale=0.9, position=(5, 5))
    timer = 0
    playing = True
    score = 0  # Oyunu başlatınca skoru sıfırlıyoruz
    if menu_ui: menu_ui.enabled = False
    if game_over_ui: game_over_ui.enabled = False

def game_over():
    global playing
    playing = False
    show_game_over_menu()

def show_start_menu():
    global menu_ui
    menu_ui = Entity(enabled=True)
    Text("YILAN OYUNU", parent=menu_ui, scale=30, y=0.3)  # Emojiler kaldırıldı ve scale artırıldı

def show_game_over_menu():
    global game_over_ui
    game_over_ui = Entity()
    Text("OYUN BITTI", parent=game_over_ui, scale=30, y=0.3)  # Emojiler kaldırıldı ve scale artırıldı
    Text(f"Skorunuz: {score}", parent=game_over_ui, scale=2, y=-0.1)  # Skor burada gösterilecek
    Text("Yeniden başlamak için [Enter] tuşuna bas", parent=game_over_ui, y=-0.3, scale=1.5)

def update():
    global timer, direction, score

    # Menü kontrolleri
    if not playing:
        if held_keys['enter']:
            start_game()
        return

    if held_keys['w'] and direction != Vec2(0, -1): direction = Vec2(0, 1)
    if held_keys['s'] and direction != Vec2(0, 1): direction = Vec2(0, -1)
    if held_keys['a'] and direction != Vec2(1, 0): direction = Vec2(-1, 0)
    if held_keys['d'] and direction != Vec2(-1, 0): direction = Vec2(1, 0)

    timer += time.dt
    if timer >= 0.15:
        timer = 0
        new_pos = snake[0].position + Vec3(direction.x, direction.y, 0)

        for parca in snake[1:]:
            if parca.position == new_pos:
                game_over()
                return

        for i in range(len(snake)-1, 0, -1):
            snake[i].position = snake[i-1].position
        snake[0].position = new_pos

        if snake[0].position == food.position:
            add_tail()
            move_food()
            score += 1  # Skoru arttır

    # Skoru sürekli güncelle
    score_text.text = f"Skor: {score}"

# Skor göstergesi
score_text = Text(text=f"Skor: {score}", position=(-0.45, 0.4), scale=2)

show_start_menu()
app.run()
