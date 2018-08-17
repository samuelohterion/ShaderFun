#-------------------------------------------------
#
# Project created by QtCreator 2017-11-24T10:00:00
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EZGL
TEMPLATE = app
CONFIG += c++11

SOURCES += \
	code/main.cpp \
	code/widget.cpp \
	glprojects/cube/cube.cpp \
	glprojects/funIn2D/funIn2D.cpp \
	glprojects/lighttest/lighttest.cpp \
	glprojects/coordinateSystem/coordinatesystem.cpp \
	glprojects/simpleTriangle/simpletriangle.cpp \
	glprojects/sphere/sphere.cpp \
	glprojects/spheredynamicsurface/spheredynamicsurface.cpp \
	glprojects/simplecoloredtriangle/simplecoloredtriangle.cpp

HEADERS += \
	code/widget.hpp \
	code/clock.hpp  \
	code/glrenderer.hpp \
	glprojects/cube/cube.hpp \
	glprojects/funIn2D/funIn2D.hpp \
	glprojects/lighttest/lighttest.hpp \
	glprojects/coordinateSystem/coordinatesystem.hpp \
	glprojects/simpleTriangle/simpletriangle.hpp \
	glprojects/sphere/sphere.hpp \
	glprojects/spheredynamicsurface/spheredynamicsurface.hpp \
	glprojects/simplecoloredtriangle/simplecoloredtriangle.hpp

FORMS += \
	code/widget.ui

DISTFILES += \
	glprojects/cube/cube.vsh \
	glprojects/cube/cube.fsh \
	glprojects/cube/wall.vsh \
	glprojects/cube/wall.fsh \
	glprojects/funIn2D/fun1.vsh \
	glprojects/funIn2D/fun1.fsh