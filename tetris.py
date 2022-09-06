import pgzrun
import pgzero
from pgzero.builtins import Rect, keys, clock, keyboard
screen : pgzero.screen.Screen
from random import randint


TITLE = "Tetris"
WIDTH = 660
HEIGHT = 600
SIZE = 30
ICON = "tetris1.png"


posx = 0
posy = 0
board_width = 10
board_height = 20
offset_x = 30
level = 1
score = 0
lines = 0
tick = 0
delay = 1 / 60 * (53 - 5 * level)
game_state = "over"


O_Tetrimino = [
    [1, 1],
    [1, 1]]

I_Tetrimino = [
    [0, 1, 0, 0],
    [0, 1, 0, 0],
    [0, 1, 0, 0],
    [0, 1, 0, 0]]

T_Tetrimino = [
    [0, 1, 0],
    [1, 1, 0],
    [0, 1, 0]]

L_Tetrimino = [
    [1, 1, 0],
    [0, 1, 0],
    [0, 1, 0]]

J_Tetrimino = [
    [0, 1, 0],
    [0, 1, 0],
    [1, 1, 0]]

S_Tetrimino = [
    [1, 0, 0],
    [1, 1, 0],
    [0, 1, 0]]

Z_Tetrimino = [
    [0, 1, 0],
    [1, 1, 0],
    [1, 0, 0]]

shapes = [O_Tetrimino, I_Tetrimino, T_Tetrimino, L_Tetrimino, J_Tetrimino, S_Tetrimino, Z_Tetrimino]


def random_color():
    color = [(randint(20, 255), randint(20, 255), randint(20, 255))]
    color.append((color[0][0] / 2, color[0][1] / 2, color[0][2] / 2))
    return color


def reset_pos():
    global posx, posy
    posy = 0
    if len(shape) == 2:
        posx = 4
    else:
        posx = 3


def reset_board():
    global board, color, shape, next_color, next_shape
    board = [[0]* board_height for i in range(board_width)]
    color = random_color()
    shape = shapes[randint(0, 6)]
    reset_pos()
    next_color = random_color()
    next_shape = shapes[randint(0, 6)]


reset_board()


def reset_game():
    global level, score, lines, tick, delay, game_state
    reset_board()
    level = 1
    score = 0
    lines = 0
    tick = 0
    delay = 1 / 60 * (53 - 5 * level)
    game_state = "play"
    clock.schedule_interval(game_update, delay / 4)


def remove_row(column):
    for row in range(board_width):
        board[row][column] = 0
    for y in range(column - 1, 1, -1):
        for x in range(board_width):
            if board[x][y]:
                 board[x][y + 1] = board[x][y]
                 board[x][y] = 0


def check_row(row):
    for i in row:
        if not i:
            return False
    return True


def update_score(cleared_lines):
    global score
    if cleared_lines == 1:
        score += 40 * level
    elif cleared_lines == 2:
        score += 100 * level
    elif cleared_lines == 3:
        score += 300 * level
    elif cleared_lines == 4:
        score += 1200 * level


def update_level():
    global lines, level, delay
    if lines >= level * 10:
        lines -= level * 10
        level += 1
        reset_board()
        if level < 10:
            delay = 1 / 60 * (53 - 5 * level)
        elif level == 10:
            delay = 1 / 60 * 6
        elif level < 14:
            delay = 1 / 60 * 5
        elif level < 17:
            delay = 1 / 60 * 4
        elif level < 20:
            delay = 1 / 60 * 3
        elif level < 30:
            delay = 1 / 60 * 2
        else:
            delay = 1 / 60 * 1


def check_board():
    global lines, level
    cleared_lines = 0
    for column in range(board_height):
        test_row = list(zip(*board))[column]
        if check_row(test_row):
            remove_row(column)
            cleared_lines += 1
    update_score(cleared_lines)
    lines += cleared_lines
    update_level()


def shape_rotate():
    global shape
    w = len(shape)
    newshape = [[0]* w for i in range(w)]
    for x in range(w):
        for y in range(w):
            if shape[x][y]:
                if w - 1 - y + posx >= board_width or w - 1 - y + posx < 0 or x + posy >= board_height:
                    return
                if board[w - 1 - y + posx][posy + x]:
                    return
            newshape[w - 1 - y][x] = shape[x][y]
    shape = newshape


def check_collide(newx, newy):
    for x in range(len(shape)):
        for y in range(len(shape[x])):
            if shape[x][y]:
                if x + newx >= board_width or x + newx < 0 or y + newy >= board_height:
                    return True
                if board[x + newx][y + newy]:
                    return True
    return False


def shape_move(direction):
    global posx, posy
    newx, newy = posx, posy
    if direction == "right":
        newx += 1
    elif direction == "left":
        newx -= 1
    elif direction == "down":
        newy += 1

    if check_collide(newx, newy):
        return False

    posx = newx
    posy = newy
    return True


def game_update():
    global next_shape, next_color, shape, color, posx, posy, tick, game_state
    if keyboard.left:
        shape_move("left")
    if keyboard.right:
        shape_move("right")
    if keyboard.down or tick > 3:
        tick = 0
        if not shape_move("down"):
            for x in range(len(shape)):
                for y in range(len(shape[x])):
                    if shape[x][y]:
                        board[x + posx][y + posy] = color
            shape = next_shape
            reset_pos()
            color = next_color
            if check_collide(posx, posy):
                game_state = "over"
                clock.unschedule(game_update)
            next_shape = shapes[randint(0, 6)]
            next_color = random_color()
            check_board()
    else:
        tick += 1


def on_key_down(key):
    global shape, color, posx, posy
    if game_state == "play":
        if key == keys.UP:
            shape_rotate()
        elif key == keys.RIGHT:
            shape_move("right")
        elif key == keys.LEFT:
            shape_move("left")
        elif key == keys.DOWN:
            clock.unschedule(game_update)
            clock.schedule_interval(game_update, delay / 4)
            shape_move("down")
    else:
        if key == keys.SPACE:
            reset_game()


def draw_box(posx, posy, color1, color2):
    box = Rect((posx * SIZE + offset_x, posy * SIZE), (SIZE, SIZE))
    box.x += 2
    box.y += 2
    box.width -= 4
    box.height -= 4
    screen.draw.filled_rect(box , color2)
    screen.draw.rect(box , color1)
    box.x += 2
    box.y += 2
    box.width -= 4
    box.height -= 4
    screen.draw.filled_rect(box , color2)
    box.x += 3
    box.y += 3
    box.width -= 6
    box.height -= 6
    screen.draw.filled_rect(box , color1)
    box.x += 5
    box.y += 5
    box.width -= 10
    box.height -= 10
    screen.draw.filled_rect(box , color2)


def draw_text():
    screen.draw.text("Score", midbottom=(510, 60), color="red", fontsize=60)
    screen.draw.text(str(score), midtop=(510, 60), color="orange", fontsize=60)
    screen.draw.text("Level", midbottom=(510, 180), color="yellow", fontsize=60)
    screen.draw.text(str(level), midtop=(510, 180), color="green", fontsize=60)
    screen.draw.text("Lines", midbottom=(510, 420), color="blue", fontsize=60)
    screen.draw.text(str(lines), midtop=(510, 420), color="purple", fontsize=60)
    screen.draw.text("s", bottomright=(649, HEIGHT), owidth=1, ocolor="purple", color="darkslateblue", fontsize=140)
    screen.draw.text("i", bottomright=(591, HEIGHT), owidth=1, ocolor="blue", color="darkblue", fontsize=140)
    screen.draw.text("r", bottomright=(564, HEIGHT), owidth=1, ocolor="green", color="darkgreen", fontsize=140)
    screen.draw.text("t", bottomright=(520, HEIGHT), owidth=1, ocolor="yellow", color="goldenrod", fontsize=140)
    screen.draw.text("e", bottomright=(488, HEIGHT), owidth=1, ocolor="orange", color="chocolate", fontsize=140)
    screen.draw.text("T", bottomright=(442, HEIGHT), owidth=1, ocolor="red", color="darkred", fontsize=140)
    if game_state == "over":
        screen.draw.text("Press", midbottom=(180, 300), color="white", fontsize=120)
        screen.draw.text("SPACE", midtop=(180, 300), color="white", fontsize=120)


def draw_shape():
    length = len(shape)
    for x in range(length):
        for y in range(length):
            if shape[x][y]:
                draw_box(x + posx, y + posy, color[0], color[1])
    length = len(next_shape)
    for x in range(length):
        for y in range(length):
            if next_shape[x][y]:
                draw_box(x + 16 - length / 2, y + 10 - length / 2, next_color[0], next_color[1])


def draw_board():
    box = Rect((0, 0), (360, HEIGHT))
    screen.draw.filled_rect(box , color="purple")
    box.x += 5
    box.width -= 10
    screen.draw.filled_rect(box , color="blue")
    box.x += 5
    box.width -= 10
    screen.draw.filled_rect(box , color="green")
    box.x += 5
    box.width -= 10
    screen.draw.filled_rect(box , color="yellow")
    box.x += 5
    box.width -= 10
    screen.draw.filled_rect(box , color="orange")
    box.x += 5
    box.width -= 10
    screen.draw.filled_rect(box , color="red")
    box.x += 5
    box.width -= 10
    screen.draw.filled_rect(box , color="black")
    for x, column in enumerate(board):
        for y, value in enumerate(column):
            if value:
                draw_box(x, y, value[0], value[1])


def update():
    pass


def draw():
    screen.clear()
    draw_board()
    draw_shape()
    draw_text()


pgzrun.go()