#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>

using namespace std;
// SETUP
int figures[7][4] =
{
	1,3,5,7,
	2,4,6,7,
	3,5,7,6,
	2,4,5,6,
	2,4,5,7,
	3,5,4,6,
	2,3,4,5,
};

int const M = 30;
int const N = 20;
int field[M][N] = {0};

vector<sf::Vector2i> pieces (4), a(4);

// CREATE NEW BLOCK POSITION
int create_NewBlockPosition()
{
	int rand_pos = rand()%20;
	int random = rand()%7;
	for (int i = 0;i < 4; i++)
	    {
            pieces[i].x = figures[random][i] % 2 + 8;
		    pieces[i].y = figures[random][i] / 2 - 5;
        }
        return rand_pos;
}

// DRAW BLOCK
sf::Texture block_image;
sf::Sprite block;
sf::Texture bg_image;
sf::Sprite background;

void drawBlock(sf::RenderWindow &window, int isSpecial)
{
	block_image.loadFromFile("game_images\\tiles.png");
	block.setTexture(block_image);
	block.setTextureRect(sf::IntRect(0, 0, 18, 18));
	bg_image.loadFromFile("game_images\\bg.jpg");
	background.setTexture(bg_image);
	window.draw(background);

	for (int i = 0; i < 4; i++)
	{
		if (i == isSpecial)
		block.setColor(sf::Color::Red);
		block.setPosition(pieces[i].x*18, pieces[i].y*18);
		window.draw(block);
		block.setColor(sf::Color::Yellow);
	}
	for(int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
	    {
	    	if(field[i][j] == 2)
			{
				block.setColor(sf::Color::Red);
				block.setPosition(j*18,i*18);
				window.draw(block);
	    	}
		    if(field[i][j] == 1)
		    {
		    	block.setColor(sf::Color::Yellow);
				block.setPosition(j*18,i*18);
				window.draw(block);
		    }
	    }
	}
}

// ROTATION
bool check()
{
	for (int i = 0; i < 4; i++)
	{
		int x = pieces[i].y;
		int y = pieces[i].x;
		if (field[x][y] != 0 || y < 0 || y > N-1 || x >= M-1) return false;
	}
	return true;
}

void rotation ()
{
	sf::Vector2i center;
	center = pieces[1];
	for (int i = 0; i < 4; i++)
	{
		int x = pieces[i].y - center.y;
		int y = pieces[i].x - center.x;
		pieces[i].x = center.x - x;
		pieces[i].y = center.y + y;
	}
	if (check() == false)
	{
		for (int i = 0; i < 4; i++) pieces[i] = a[i];
	}
}


// MOVE BLOCK

void moveBlock (int dx)
{
	for (int i = 0; i < 4; i++)
	{
		a[i] = pieces[i];
		pieces[i].x += dx;
	}
	if (check() == false)
	{
		for (int i = 0; i < 4; i++) pieces[i] = a[i];
	}
}

// DROP
bool colision = false;

void drop(int isSpecial)
{
	for (int i = 0; i < 4; i++)
	{
		int x = pieces[i].x;
		int y = pieces[i].y;
		if (y + 1 > M-1 || field[y+1][x] != 0) colision = true;
	}
	if (colision == false)
	{
		for (int i = 0; i < 4; i++) pieces[i].y ++;
	}
	else
	{
		for (int i = 0; i < 4; i++)
	    {
	    	if(i == isSpecial)
				field[pieces[i].y][pieces[i].x] = 2;
			else
				field[pieces[i].y][pieces[i].x] = 1;
	    }
	}
}

// REMOVE LINES
int SCORE = 0;
int red_num = 0;

void remove_lines()
{
	for (int i = M-1; i > 0; i--)
{
	int count = 0;
	for (int j = 0; j < N; j++)
	{
		if (field[i][j] != 0) count ++;
	}
	if (count == N)
	{
		for (int j = 0; j < N; j++)
		{
			if (field[i][j] == 2)
			{
				red_num++;
				SCORE += 50;
			}
			else SCORE += 10;
			if (field[i-1][j] == 2)
			{
				field[i][j] = 0;
				continue;
			}
			for (int m = i; m > 0; m--)
			{
				if (field[m-1][j] == 0 && field[m-2][j] == 2) break;
				field[m][j] = field[m-1][j];
			}
		}
	}
}
}

// SCORE
sf::Font font;

void Score(sf::RenderWindow &window)
{
	char a[6];
	char b[6];

	font.loadFromFile("menu_font.ttf");
	sf::Text text;
	sf::Text RED;

	text.setFont(font);
	text.setPosition(0, 5);
	text.setString("SCORE: " + (string)itoa(SCORE, a, 10));
	text.setCharacterSize(25);
	text.setColor(sf::Color::Yellow);

	RED.setFont(font);
	RED.setPosition(190, 10);
	RED.setCharacterSize(20);
	RED.setString("RED STONES: " + (string)itoa(red_num, b, 10));

	window.draw(RED);
	window.draw(text);
}

// SKILL 1
void Skill_1()
{
	if (red_num >= 3)
	{
	    for (int i = M-1; i > 0; i--)
	    {
		     for (int j = 0; j < N; j++)
		    {
				if (field[i][j] == 2)
			    {
			    	field[i][j] = 0;
					for (int n = i; n < M; n++)
				    {
						for (int m = n; m > 0; m--) field[m][j] = field[m-1][j];
					    if (field[n+1][j] != 0 || n+1 > M-1) break;
				    }
			    }
			}
	    }
	    red_num = red_num - 3;
	}
}

// MENU
sf::Texture menu_image;
sf::Sprite menu;

void Menu(sf::RenderWindow &window)
{
	menu_image.loadFromFile("game_images\\menu.jpg");
    menu.setTexture(menu_image);
	font.loadFromFile("menu_font.ttf");
	sf::Text menu_text ("BLOCK - BREAKING", font);
	sf::Text note ("Tap enter to start !", font);

	menu_text.setPosition(25,200);
	menu_text.setColor(sf::Color::Black);
	menu_text.setOutlineColor(sf::Color::Red);
	menu_text.setOutlineThickness(2);

	note.setPosition(30,300);
	note.setOutlineColor(sf::Color::Blue);
	note.setOutlineThickness(2);

    window.draw(menu);
	window.draw(menu_text);
	window.draw(note);
	window.display();
}

//MUSIC
sf::Music bg_music;

void music()
{
	bg_music.openFromFile("tetris_music.wav");
	bg_music.setLoop(true);
	bg_music.play();
}

// GAME OVER
bool GameOver(sf::RenderWindow &window)
{
	for (int i = 0; i < N; i++)
	{
		if (field[0][i] != 0)
		{
			menu_image.loadFromFile("game_images\\menu.jpg");
            menu.setTexture(menu_image);
	        font.loadFromFile("menu_font.ttf");
	        sf::Text gameover ("YOU LOSE !!!", font);
	        sf::Text note2 ("Tap enter to try again !", font);

	        gameover.setPosition(100, 200);
	        gameover.setColor(sf::Color::Red);
	        gameover.setCharacterSize(30);
	        gameover.setOutlineColor(sf::Color::Black);
	        gameover.setOutlineThickness(3);

	        note2.setPosition(20, 300);
	        note2.setCharacterSize(28);
	        note2.setColor(sf::Color::Green);
	        note2.setOutlineColor(sf::Color::Red);
	        note2.setOutlineThickness(3);

	        window.draw(menu);
	        window.draw(gameover);
	        window.draw(note2);
	        window.display();

			return true;
		}
	}
	return false;
}
// MAIN
int main()
{
	srand(time(0));
    sf::RenderWindow window(sf::VideoMode(360, 540), "Block-Breaking", sf::Style::None);
    window.setFramerateLimit(60);
    while (window.isOpen())
	{
		sf::Event e;
		music();
		while (window.pollEvent(e))
		{
		}
		Menu(window);

// NEW GAME
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
	int countdown = 0;
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
// GAME LOOP
       // GameOver(window);
        int isSpecial = create_NewBlockPosition();
        colision = false;
		while (GameOver(window) == false)
		{
			sf::Event event;
			int dx = 0, dy = 0;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) window.close();
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Up)  rotation();
					else if (event.key.code == sf::Keyboard::Left) dx = -1;
			        else if (event.key.code == sf::Keyboard::Right) dx = 1;
			        else if (event.key.code == sf::Keyboard::Down) dy = 1;
			        moveBlock(dx);
			        if(event.key.code == sf::Keyboard::Num1) Skill_1();
				}
			}
			if(countdown-- <= 0 || dy == 1)
			{
			    drop(isSpecial);
			    countdown = 2;
			}
			remove_lines();
			window.clear();
			drawBlock(window, isSpecial);
			Score(window);
			window.display();
			if (colision == true) break;
		}
	}
	}
	}
    return 0;
}