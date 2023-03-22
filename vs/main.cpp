#include <SFML\Graphics.hpp>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace sf;

const int h = 21, w = 19, ts = 25, ghosts = 2;
int score = 0;
bool life = true;

int x[4] = { 1, 17, 1, 17 }, y[4] = { 1, 19, 19, 1 };
int newX[4] = { 0, 0, 0, 0 }, newY[4] = { 0, 0, 0, 0 };
int rotate[4] = { 1, 1, 1, 1 };
int facing[4] = { 1, 1, 1, 1 }; // 1 - right, 2 - left, 3 - up, 4 - down
bool change[4][3]; // 1 - right, 2 - left, 3 - forward

int xP = 9, yP = 15;

float dt;

String map[h] = {
	"AAAAAAAAAAAAAAAAAAA",
	"A        A        A",
	"A AA AAA A AAA AA A",
	"A        A        A",
	"A AA A AAAAA A AA A",
	"A    A   A   A    A",
	"AAAA AAA A AAA AAAA",
	"BBBA A       A ABBB",
	"AAAA A AAAAA A AAAA",
	"BBBB   ABBBA   BBBB",
	"AAAA A AAAAA A AAAA",
	"BBBA A       A ABBB",
	"AAAA A AAAAA A AAAA",
	"A        A        A",
	"A AA AAA A AAA AA A",
	"A  A     B     A  A",
	"AA A A AAAAA A A AA",
	"A    A   A   A    A",
	"A AAAAAA A AAAAAA A",
	"A                 A",
	"AAAAAAAAAAAAAAAAAAA",
};

void yPlus(int i, int p) {
	if (map[newY[i] + 1][x[i]] != 'A') {
		if (map[newY[i] + 1][x[i]] == 'C') {
			life = false;
		}
		else {
			change[i][p] = true;
		}
	}
	else change[i][p] = false;
}

void yMinus(int i, int p) {
	if (map[newY[i] - 1][x[i]] != 'A') {
		if (map[newY[i] - 1][x[i]] == 'C') {
			life = false;
		}
		else {
			change[i][p] = true;
		}
	}
	else change[i][p] = false;
}

void xPlus(int i, int p) {
	if (map[y[i]][newX[i] + 1] != 'A') {
		if (map[y[i]][newX[i] + 1] == 'C') {
			life = false;
		}
		else {
			change[i][p] = true;
		}
	}
	else change[i][p] = false;
}

void xMinus(int i, int p) {
	if (map[y[i]][newX[i] - 1] != 'A') {
		if (map[y[i]][newX[i] - 1] == 'C') {
			life = false;
		}
		else {
			change[i][p] = true;
		}
	}
	else change[i][p] = false;
}

class Player {
public:
	float frame = 0;
	int newXP = 0, newYP = 0, rotate = 1, ti = 0;

	void update() {
		frame += 13 * dt;
		if (frame > 5) {
			frame -= 5;
		}

		ti++;
		if (ti >= 900 * dt) {
			newXP = xP;
			newYP = yP;

			switch (rotate) {
			case 1:
				if (map[yP][newXP + 1] != 'A') {
					newXP++;
				}
				break;
			case 2:
				if (map[yP][newXP - 1] != 'A') {
					newXP--;
				}
				break;
			case 3:
				if (map[newYP - 1][xP] != 'A') {
					newYP--;
				}
				break;
			case 4:
				if (map[newYP + 1][xP] != 'A') {
					newYP++;
				}
				break;
			}
			ti = 0;

			for (int i = 0; i < ghosts; i++) {
				if ((x[i] == newXP && y[i] == newYP) || (x[i] == xP && y[i] == yP)) {
					life = false;
				}
			}

			if (life) {
				if (map[newYP][newXP] == ' ') {
					map[newYP][newXP] = 'B';
					score++;
				}
				xP = newXP;
				yP = newYP;
			}
			if (newYP == 9 && (newXP == 0 || newXP == 18)) {
				if (newXP == 0) {
					newXP = 17;
				}
				else {
					newXP = 1;
				}
				xP = newXP;
				yP = newYP;
			}
		}
	}
};

class Enemy {
public:
	int ti = 0;
	void update() {
		ti++;
		if (ti >= 900 * dt) {
			for (int i = 0; i < ghosts && life; i++) {
				newX[i] = x[i];
				newY[i] = y[i];

				switch (facing[i]) {
				case 1:
					yPlus(i, 0);
					yMinus(i, 1);
					xPlus(i, 2);
					break;
				case 2:
					yMinus(i, 0);
					yPlus(i, 1);
					xMinus(i, 2);
					break;
				case 3:
					xPlus(i, 0);
					xMinus(i, 1);
					yMinus(i, 2);
					break;
				case 4:
					xMinus(i, 0);
					xPlus(i, 1);
					yPlus(i, 2);
					break;
				}

				if (x[i] == xP && y[i] == yP) {
					life = false;
				}

				if (!life) break;

				std::vector<int> chose;
				for (int j = 0; j < 3; j++) {
					if (change[i][j]) {
						chose.push_back(j);
					}
				}
				rotate[i] = chose[rand() % chose.size()] + 1;

				switch (facing[i]) {
				case 1:
					if (rotate[i] == 1) {
						newY[i]++;
						facing[i] = 4;
					}
					else if (rotate[i] == 2) {
						newY[i]--;
						facing[i] = 3;
					}
					else {
						newX[i]++;
					}
					break;
				case 2:
					if (rotate[i] == 1) {
						newY[i]--;
						facing[i] = 3;
					}
					else if (rotate[i] == 2) {
						newY[i]++;
						facing[i] = 4;
					}
					else {
						newX[i]--;
					}
					break;
				case 3:
					if (rotate[i] == 1) {
						newX[i]++;
						facing[i] = 1;
					}
					else if (rotate[i] == 2) {
						newX[i]--;
						facing[i] = 2;
					}
					else {
						newY[i]--;
					}
					break;
				case 4:
					if (rotate[i] == 1) {
						newX[i]--;
						facing[i] = 2;
					}
					else if (rotate[i] == 2) {
						newX[i]++;
						facing[i] = 1;
					}
					else {
						newY[i]++;
					}
					break;
				}

				if (newY[i] == 9 && (newX[i] == 0 || newX[i] == 18)) {
					if (newX[i] == 0) {
						newX[i] = 17;
					}
					else {
						newX[i] = 1;
					}
				}
				x[i] = newX[i];
				y[i] = newY[i];
			}
			ti = 0;
		}
	}
};

int main() {
	srand(time(NULL));
	RenderWindow window(VideoMode(w * ts, h * ts), "Pack-man");
	window.setFramerateLimit(60);

	Texture t;
	if (!t.loadFromFile("images/title.png")) {
		std::cout << "Texture loading failed";
		return -1;
	}
	Sprite plat(t);

	Texture winT;
	if (!winT.loadFromFile("images/youwin.png")) {
		std::cout << "Texture loading failed";
		return -1;
	}
	Sprite win(winT);

	Texture loseT;
	if (!loseT.loadFromFile("images/youlose.png")) {
		std::cout << "Texture loading failed";
		return -1;
	}
	Sprite lose(loseT);
	std::vector<Sprite> v;

	Player p;
	Enemy en;
	
	Clock clock_dt;
	while (window.isOpen()) {
		dt = clock_dt.restart().asSeconds();
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed:
				window.close();
				return 0;
			case Event::KeyPressed:
				if (life && score < 175) {
					p.newXP = xP;
					p.newYP = yP;

					if (event.key.code == Keyboard::Right) {
						p.rotate = 1;
					}
					else if (event.key.code == Keyboard::Left) {
						p.rotate = 2;
					}
					else if (event.key.code == Keyboard::Up) {
						p.rotate = 3;
					}
					else if (event.key.code == Keyboard::Down) {
						p.rotate = 4;
					}
				}
				break;
			}
		}
		window.clear(Color::Black);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (map[i][j] == 'A') {
					plat.setTextureRect(IntRect(0, 0, ts, ts));
				}
				else if (map[i][j] == ' ') {
					plat.setTextureRect(IntRect(ts, 0, ts, ts));
				}
				else if (map[i][j] == 'B') {
					continue;
				}
				plat.setPosition(j * ts, i * ts);
				window.draw(plat);
			}
		}
		plat.setTextureRect(IntRect(ts * int(p.frame), ts * p.rotate, ts, ts));
		plat.setPosition(xP * ts, yP * ts);
		window.draw(plat);
		for (int i = 0; i < ghosts; i++) {
			plat.setTextureRect(IntRect(ts * 5, ts * facing[i], ts, ts));
			plat.setPosition(x[i] * ts, y[i] * ts);
			window.draw(plat);
		}

		if (life && score < 175) {
			p.update();
			en.update();
		}

		if (score == 175) {
			win.setOrigin(129.5, 26.5);
			win.setPosition(237.5, 230.5);
			win.setColor(Color::Green);
			window.draw(win);
		}
		if (!life) {
			lose.setOrigin(133.5, 26.5);
			lose.setColor(Color::Red);
			lose.setPosition(237.5, 262.5);
			window.draw(lose);
		}
		window.display();
	}
}