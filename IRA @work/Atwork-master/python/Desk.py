import pygame

class Desk():
    def __init__(self, x, y, direction, index):
        self.index = index
        self.x = x
        self.y = y
        self.direction = direction
    def show(self, display):
        pygame.draw.rect(display, (0,0,200),(self.x - 10, self.y - 10, 20, 20))
        font = pygame.font.SysFont("serif", 16)
        text = font.render(str(self.index), True, (255,255,255))
        display.blit(text, (self.x-4, self.y-10))
