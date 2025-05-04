from ursina import *
import random

app = Ursina()

map_limit = 25

# Oyuncu
player = Entity(model='cube', color=color.azure, scale=(1, 1, 1), collider='box')
player.health = 5
player.invincible = 0
player.xp = 0  # XP özelliği
player.level = 1  # Seviye özelliği

# Zemin
ground = Entity(model='plane', scale=map_limit*2, texture='white_cube', texture_scale=(50, 50), collider='box')

# Kamera
camera.position = (0, 30, 0)
camera.rotation_x = 90

# Harita sınırları
walls = [
    Entity(position=(map_limit+0.5, 0, 0), scale=(1, 5, map_limit*2), collider='box', visible=False),
    Entity(position=(-map_limit-0.5, 0, 0), scale=(1, 5, map_limit*2), collider='box', visible=False),
    Entity(position=(0, 0, map_limit+0.5), scale=(map_limit*2, 5, 1), collider='box', visible=False),
    Entity(position=(0, 0, -map_limit-0.5), scale=(map_limit*2, 5, 1), collider='box', visible=False),
]

# Can yazısı
health_text = Text(
    text=f'Can: {player.health}',
    position=window.top_left + Vec2(0.05, -0.02),
    scale=1.2,
    origin=(0, 0),
    color=color.black,
    background=True
)
health_text.background_color = color.white66  # Hafif gri arka plan

# Seviye yazısı
level_text = Text(
    text=f'Seviye: {player.level}',
    position=window.top_left + Vec2(0.05, -0.05),
    scale=1.2,
    origin=(0, 0),
    color=color.black,
    background=True
)

# Ölüm yazısı
death_text = Text('', position=Vec2(0, 0), scale=2, origin=(0, 0), color=color.red)

# Düşman sınıfı
class Enemy(Entity):
    def __init__(self, position=(0, 0, 0)):
        super().__init__(
            model='cube',
            color=color.red,
            scale=(1, 1, 1),
            position=position,
            collider='box'
        )
        self.speed = 2

    def update(self):
        if death_text.text != '':
            return
        direction = (player.position - self.position).normalized()
        self.position += direction * self.speed * time.dt
        self.look_at_2d(player, axis='y')

        if distance(self.position, player.position) < 1 and player.invincible <= 0:
            player.health -= 1
            player.invincible = 1
            health_text.text = f'Can: {player.health}'
            print("Vuruldun! Can:", player.health)

            if player.health <= 2:
                health_text.color = color.red
            else:
                health_text.color = color.black

            if player.health <= 0:
                death_text.text = 'ÖLDÜN!'
                player.disable()

# Boss sınıfı
class Boss(Entity):
    def __init__(self, position=(0, 0, 0)):
        super().__init__(
            model='cube',
            color=color.orange,
            scale=(2, 2, 2),
            position=position,
            collider='box'
        )
        self.health = 5
        self.speed = 3

    def update(self):
        if death_text.text != '':
            return
        direction = (player.position - self.position).normalized()
        self.position += direction * self.speed * time.dt
        self.look_at_2d(player, axis='y')

        if distance(self.position, player.position) < 1 and player.invincible <= 0:
            player.health -= 1
            player.invincible = 1
            health_text.text = f'Can: {player.health}'
            print("Vuruldun! Can:", player.health)

            if player.health <= 2:
                health_text.color = color.red
            else:
                health_text.color = color.black

            if player.health <= 0:
                death_text.text = 'ÖLDÜN!'
                player.disable()

# Düşman listesi
enemies = []

def spawn_enemy():
    x = random.uniform(-map_limit + 1, map_limit - 1)
    z = random.uniform(-map_limit + 1, map_limit - 1)
    enemies.append(Enemy(position=(x, 0, z)))

for _ in range(3):
    spawn_enemy()

spawn_timer = 0
spawn_interval = 5
max_enemies = 10

# Dash
is_dashing = False
dash_direction = Vec3(0, 0, 0)
dash_speed = 20
dash_duration = 0.15
dash_timer = 0

def input(key):
    global is_dashing, dash_direction, dash_timer
    if key == 'space' and not is_dashing:
        is_dashing = True
        dash_direction = Vec3(0, 0, 0)
        if held_keys['w']: dash_direction += Vec3(0, 0, 1)
        if held_keys['s']: dash_direction += Vec3(0, 0, -1)
        if held_keys['a']: dash_direction += Vec3(-1, 0, 0)
        if held_keys['d']: dash_direction += Vec3(1, 0, 0)
        if dash_direction == Vec3(0, 0, 0): dash_direction = Vec3(0, 0, 1)
        dash_direction = dash_direction.normalized()
        dash_timer = dash_duration

        player.invincible = dash_duration  # Dash boyunca dokunulmaz

def update():
    global spawn_timer, is_dashing, dash_timer

    if death_text.text != '':
        return

    # WASD hareket
    speed = 5 * time.dt
    move = Vec3(
        held_keys['d'] - held_keys['a'],
        0,
        held_keys['w'] - held_keys['s']
    ).normalized() * speed
    player.position += move

    # Sınırlar
    player.x = clamp(player.x, -map_limit, map_limit)
    player.z = clamp(player.z, -map_limit, map_limit)

    # Kamera takip
    camera.x = player.x
    camera.z = player.z

    # Düşmanlar
    for enemy in enemies:
        enemy.update()

    # Düşman spawn
    spawn_timer += time.dt
    if spawn_timer >= spawn_interval:
        spawn_timer = 0
        if len(enemies) < max_enemies:
            spawn_enemy()

    # Dash
    if is_dashing:
        player.position += dash_direction * dash_speed * time.dt
        dash_timer -= time.dt

        for enemy in enemies[:]:
            if distance(player.position, enemy.position) < 1:
                enemies.remove(enemy)
                destroy(enemy)

        if dash_timer <= 0:
            is_dashing = False

    # Invincibility süresi
    if player.invincible > 0:
        player.invincible -= time.dt

    # XP kazanma
    player.xp += 1
    if player.xp >= 100:  # 100 XP seviyeye yükselmek için yeterli
        player.xp = 0
        player.level += 1
        level_text.text = f'Seviye: {player.level}'
        print(f'Yeni Seviye: {player.level}')

        if player.level == 10:  # Seviye 10'da boss ekleme
            boss = Boss(position=(random.uniform(-map_limit+1, map_limit-1), 0, random.uniform(-map_limit+1, map_limit-1)))
            enemies.append(boss)
            print("Seviye 10'a ulaştın! Boss savaşı başlıyor.")

app.run()
