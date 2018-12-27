#include <type.hpp>
#include <graphs.hpp>
#include <fields.hpp>
#include <wav.hpp>

int if_fresh = 1, if_draw = 0;
list<Shape *> Window::shapes = {};
map<string, View2D *> Window::menus = {};
vector<Field*> fields;
unsigned long fresh_screen(int signal);

FILE *fp;
Window *p_nwd = NULL;

WAV twav("data/test.wav");
Shape *p_avg = new Shape();

int main(int argc, char * argv[]) {
	Window nwd(argc, argv);
	p_nwd = &nwd;
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	int hw = init_test_wav(120);
	create_fields(hw);
	nwd.create("Fields");
	nwd.show();

	return 0;
}