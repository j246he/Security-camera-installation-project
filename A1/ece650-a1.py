#!/usr/bin/env python
import sys
import re
# YOUR CODE GOES HERE
class ParseException(Exception):
    def __init__(self, value):
        self._value = value

    def __str__(self):
        return str(self._value)

def parse_line(line):
    if not line.startswith('a'):
        if not line.startswith('c'):
            if not line.startswith('r'):
                if not line.startswith('g'):
                    raise ParseException("does not start with 'a' or 'c' or 'r'!")
                else:
                    return line
            else:
                return line
        else:
            return line
    else:
        return line


def run(input_point,input, out, err):
    input_street=[]
    while True:
        line = input.readline()
        active='active'
        if line == '':
            active='1'
            flag='right'
            return input_point,input_street,active,flag
        try:
            input_street.append(parse_line(line))#an input sentence
            if check_point(input_street[-1]):
                if input_street[-1].startswith('a'):
                    if len(input_street)>1:
                        for streets in input_street[:-1]:
                            if streets.title()==input_street[-1].title():
                                raise ParseException("street currently exists.")
                                return input_point,input_street,active,flag
                    flag='right'
                    input_point=street_info(input_street[-1],input_point)
                elif input_street[-1].startswith('c'):
                    flag='right'
                    input_point=change_street(input_street[-1],input_point)
                elif input_street[-1].startswith('r'):
                    flag='right'
                    input_point=delete_street(input_street[-1],input_point)
                elif input_street[-1].startswith('g'):
                    flag='right'
                    return input_point,input_street,active,flag
            else:
                raise ParseException("street currently exists.")
                return input_point,input_street,active,flag
        except ParseException as ex:
            out.write("Error: {0}\n".format(ex))
            flag='wrong'
            return input_point,input_street,active,flag

def pp(x):
    """Returns a pretty-print string representation of a number.
       A float number is represented by an integer, if it is whole,
       and up to two decimal places if it isn't
    """
    if isinstance(x, float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)

def read_point(input_point):
    r=re.compile(r'\([-]?[0-9]+,[-]?[0-9]+\)')
    point_set=r.findall(input_point)
    return point_set

def check_point(input_point):
    r=re.compile(r'\([-]?[0-9]+,[-]?[0-9]+\)')
    point_set=r.findall(input_point)
    r=re.compile(r'[-]?[0-9]+')
    point_set1=r.findall(input_point)
    r=re.compile(r'\(')
    front_brackets=r.findall(input_point)
    r=re.compile(r'\)')
    back_brackets=r.findall(input_point)
    if len(front_brackets)!=len(back_brackets):
        flag_point=False
        raise ParseException("Incorrect input format")
        sys.stdout.write("Error: {0}\n".format(ParseException))
        return flag_point
    if len(point_set)*2!=len(point_set1):
        flag_point=False
        raise ParseException("missing bracket!")
        sys.stdout.write("Error: {0}\n".format(ParseException))
        return flag_point
    else:
        flag_point=True
        return flag_point


def read_figure(point):
    r=re.compile(r'[-]?[0-9]+\.*[0-9]*')
    x,y=r.findall(point)
    return x,y

def mult(x1,y1,x2,y2,x3,y3):
    return (x1-x3)*(y2-y3)-(x2-x3)*(y1-y3)

class point(object):
    """A point in a two dimensional space"""
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __repr__(self):
        return '(' + pp(self.x) + ',' + pp(self.y) + ')'

class Line(object):
    """A line between two points"""
    def __init__(self, src, dst):
        self.src = src
        self.dst = dst

    def __repr__(self):
        return '['+ str(self.src) + '-->' + str(self.dst) + ']'

def check_intersect(l0,l1):
    x1, y1 = l0.src.x, l0.src.y
    x2, y2 = l0.dst.x, l0.dst.y
    x3, y3 = l1.src.x, l1.src.y
    x4, y4 = l1.dst.x, l1.dst.y

    if max(x1,x2) < min(x3,x4):
        return False
    if max(y1,y2) < min(y3,y4):
        return False
    if max(x3,x4)<min(x1,x2):
        return False
    if max(y3,y4)<min(y1,y2):
        return False
    if (mult(x3,y3,x2,y2,x1,y1)*mult(x2,y2,x4,y4,x1,y1)<0):
        return False
    if (mult(x1,y1,x4,y4,x3,y3)*mult(x4,y4,x2,y2,x3,y3)<0):
        return False

    return True

def coincide(l1,l2):
    point1=l1.src
    point2=l1.dst
    point3=l2.src
    point4=l2.dst
    point_final=[]
    if ((point4.x-point3.x)*(point2.y-point1.y))==((point4.y-point3.y)*(point2.x-point1.x)):
        if (point3.x-point1.x)*(point3.y-point2.y)==(point3.y-point1.y)*(point3.x-point2.x):
            if point1.x==point3.x and point1.y==point3.y:
                if point4.y==point2.y:
                    if point4.x>point2.x:
                        xcoor=point2.x
                        ycoor=point2.y
                    else:
                        xcoor=point4.x
                        ycoor=point4.y
                elif point4.x==point2.x:
                    if point4.y>point2.y:
                        xcoor=point2.x
                        ycoor=point2.y
                    else:
                        xcoor=point4.x
                        ycoor=point4.y
                else:
                    if point4.x>point2.x:
                        xcoor=point2.x
                        ycoor=point2.y
                    else:
                        xcoor=point4.x
                        ycoor=point4.y
                point_final.append(point(xcoor,ycoor))
            elif point1.x==point4.x and point1.y==point4.y:
                if point3.y==point2.y:
                    if point3.x>point2.x:
                        xcoor=point2.x
                        ycoor=point2.y
                    else:
                        xcoor=point3.x
                        ycoor=point3.y
                elif point3.x==point2.x:
                    if point3.y>point2.y:
                        xcoor=point2.x
                        ycoor=point2.y
                    else:
                        xcoor=point3.x
                        ycoor=point3.y
                else:
                    if point3.x>point2.x:
                        xcoor=point2.x
                        ycoor=point2.y
                    else:
                        xcoor=point3.x
                        ycoor=point3.y
                point_final.append(point(xcoor,ycoor))
            elif point2.x==point3.x and point2.y==point3.y:
                if point4.y==point1.y:
                    if point4.x>point1.x:
                        xcoor=point1.x
                        ycoor=point1.y
                    else:
                        xcoor=point4.x
                        ycoor=point4.y
                elif point4.x==point1.x:
                    if point4.y>point1.y:
                        xcoor=point1.x
                        ycoor=point1.y
                    else:
                        xcoor=point4.x
                        ycoor=point4.y
                else:
                    if point4.x>point1.x:
                        xcoor=point1.x
                        ycoor=point1.y
                    else:
                        xcoor=point4.x
                        ycoor=point4.y
                point_final.append(point(xcoor,ycoor))
            elif point2.x==point4.x and point2.y==point4.y:
                if point4.y==point2.y:
                    if point4.x>point2.x:
                        xcoor=point2.x
                        ycoor=point2.y
                    else:
                        xcoor=point4.x
                        ycoor=point4.y
                elif point4.x==point2.x:
                    if point4.y>point2.y:
                        xcoor=point2.x
                        ycoor=point2.y
                    else:
                        xcoor=point4.x
                        ycoor=point4.y
                else:
                    if point4.x>point2.x:
                        xcoor=point2.x
                        ycoor=point2.y
                    else:
                        xcoor=point2.x
                        ycoor=point2.y
                point_final.append(point(xcoor,ycoor))
            else:
                if point1.x==point2.x==point3.x==point4.x:
                    if max(point1.y,point2.y,point3.y,point4.y)==point1.y:
                        if min(point2.y,point3.y,point4.y)==point2.y:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point2.y,point3.y,point4.y)==point3.y:
                            xcoor=point2.x
                            ycoor=point2.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point2.y,point3.y,point4.y)==point4.y:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point2.x
                            ycoor1=point2.y
                        point_final.append(point(xcoor1,ycoor1))
                        point_final.append(point(xcoor,ycoor))
                    elif max(point1.y,point2.y,point3.y,point4.y)==point2.y:
                        if min(point1.y,point3.y,point4.y)==point1.y:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point1.y,point3.y,point4.y)==point3.y:
                            xcoor=point1.x
                            ycoor=point1.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point1.y,point3.y,point4.y)==point4.y:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point1.x
                            ycoor1=point1.y
                        point_final.append(point(xcoor1,ycoor1))
                        point_final.append(point(xcoor,ycoor))
                    elif max(point1.y,point2.y,point3.y,point4.y)==point3.y:
                        if min(point2.y,point1.y,point4.y)==point2.y:
                            xcoor=point1.x
                            ycoor=point1.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point2.y,point1.y,point4.y)==point1.y:
                            xcoor=point2.x
                            ycoor=point2.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point2.y,point1.y,point4.y)==point4.y:
                            xcoor=point1.x
                            ycoor=point1.y
                            xcoor1=point2.x
                            ycoor1=point2.y
                        point_final.append(point(xcoor1,ycoor1))
                        point_final.append(point(xcoor,ycoor))
                    elif max(point1.y,point2.y,point3.y,point4.y)==point4.y:
                        if min(point2.y,point3.y,point1.y)==point2.y:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point1.x
                            ycoor1=point1.y
                        elif min(point2.y,point3.y,point1.y)==point3.y:
                            xcoor=point2.x
                            ycoor=point2.y
                            xcoor1=point1.x
                            ycoor1=point1.y
                        elif min(point2.y,point3.y,point1.y)==point1.y:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point2.x
                            ycoor1=point2.y
                        point_final.append(point(xcoor1,ycoor1))
                        point_final.append(point(xcoor,ycoor))
                else: # point1.y==point2.y==point3.y==point4.y:
                    if max(point1.x,point2.x,point3.x,point4.x)==point1.x:
                        if min(point2.x,point3.x,point4.x)==point2.x:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point2.x,point3.x,point4.x)==point3.x:
                            xcoor=point2.x
                            ycoor=point2.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point2.x,point3.x,point4.x)==point4.x:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point2.x
                            ycoor1=point2.y
                    elif max(point1.x,point2.x,point3.x,point4.x)==point2.x:
                        if min(point1.x,point3.x,point4.x)==point1.x:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point1.x,point3.x,point4.x)==point3.x:
                            xcoor=point1.x
                            ycoor=point1.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point1.x,point3.x,point4.x)==point4.x:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point1.x
                            ycoor1=point1.y
                    elif max(point1.x,point2.x,point3.x,point4.x)==point3.x:
                        if min(point2.x,point1.x,point4.x)==point2.x:
                            xcoor=point1.x
                            ycoor=point1.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point2.x,point1.x,point4.x)==point1.x:
                            xcoor=point2.x
                            ycoor=point2.y
                            xcoor1=point4.x
                            ycoor1=point4.y
                        elif min(point2.x,point1.x,point4.x)==point4.x:
                            xcoor=point1.x
                            ycoor=point1.y
                            xcoor1=point2.x
                            ycoor1=point2.y
                    elif max(point1.x,point2.x,point3.x,point4.x)==point4.x:
                        if min(point2.x,point3.x,point1.x)==point2.x:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point1.x
                            ycoor1=point1.y
                        elif min(point2.x,point3.x,point1.x)==point3.x:
                            xcoor=point2.x
                            ycoor=point2.y
                            xcoor1=point1.x
                            ycoor1=point1.y
                        elif min(point2.x,point3.x,point1.x)==point1.x:
                            xcoor=point3.x
                            ycoor=point3.y
                            xcoor1=point2.x
                            ycoor1=point2.y
                    point_final.append(point(xcoor1,ycoor1))
                    point_final.append(point(xcoor,ycoor))
    return point_final


def intersect (l1, l2):
    """Returns a point at which two lines intersect"""
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y
    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    xcoor =  xnum / xden

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    ycoor = ynum / yden

    return point(xcoor, ycoor)

def delete_street(input_street,input_point):
    r=re.compile(r'("[A-Z]*[a-z]* *[A-Z]*[a-z]* *[A-Z]*[a-z]* *[A-Z]*[a-z]*.*")')
    pieces=r.findall(input_street)
    if pieces:
        BigString=''
        for e in pieces:
            BigString+=e+' '
        try:
            if BigString.title() not in input_point.keys():
                raise ParseException("'r' specified for a street that does not exist.")
            else:
                del input_point[BigString.title()]
                return input_point
        except ParseException as ex:
            sys.stdout.write("Error: {0}\n".format(ex))
            return input_point
    else:
        raise ParseException('The first letter of the street name is not capitalized or missed space.')
        sys.stdout.write("Error: {0}\n".format(ParseException))
        return input_point

def change_street(input_street,input_point):
    r=re.compile(r'("[A-Z]*[a-z]* *[A-Z]*[a-z]* *[A-Z]*[a-z]* *[A-Z]*[a-z]*.*")')
    pieces=r.findall(input_street)
    if pieces:
        BigString=''
        for e in pieces:
            BigString+=e+' '
        point_set=read_point(input_street)
        if point_set==[]:
            raise ParseException("'c' must be followered with coordinates.")
            sys.stdout.write("Error: {0}\n".format(ParseException))
            return input_point
        else:
            try:
                if BigString.title() not in input_point.keys():
                    raise ParseException("'c' specified for a street that does not exist.")
                else:
                    del input_point[BigString.title()]
                    input_point[BigString.title()]=point_set
                    return input_point
            except ParseException as ex:
                sys.stdout.write("Error: {0}\n".format(ex))
                return input_point
    else:
        raise ParseException('The first letter of the street name is not capitalized or missed space.')
        sys.stdout.write("Error: {0}\n".format(ParseException))
        return input_point

def street_info(input_street,input_point):
    r=re.compile(r'("[A-Z]*[a-z]* *[A-Z]*[a-z]* *[A-Z]*[a-z]* *[A-Z]*[a-z]*.*")')
    pieces=r.findall(input_street)
    if pieces:
        BigString=''
        for e in pieces:
            BigString+=e+' '
        point_set=read_point(input_street)
        if point_set==[]:
            raise ParseException("'a' must be followered with coordinates.")
            sys.stdout.write("Error: {0}\n".format(ParseException))
            return input_point
        else:
            if BigString.title() in input_point.keys():
                raise ParseException("street currently exists.")
                return input_point
            else:
                input_point[BigString.title()]=point_set
                return input_point
    else:
        raise ParseException('missed space.')
        sys.stdout.write("Error: {0}\n".format(ParseException))
        return input_point


def input_point_line_set(input_point):
    i=0
    point_set=[]
    l=[]
    line=[]
    #{'Weber Street ': ['(2,-1)', '(2,2)', '(5,5)', '(5,6)', '(3,8)'], 'King Street ': ['(4,2)', '(4,8)'], 'Davenport Road ': ['(1,4)', '(5,8)']}
    for input_streets in input_point.values():
        for points in input_streets:
            point_set.append(points)

        for i in range(0,len(point_set)-1):
            x1,y1=read_figure(point_set[i])
            x2,y2=read_figure(point_set[i+1])
            l.append(Line(point(x1,y1),point(x2,y2)))
        line.append(l)
        l=[]
        point_set=[]
    return line

def street_comparison(line):
    V={}
    E=[]
    m=0
    n=0
    k=0
    i=1
    a=1
    intersects_set=[]
    for m in range(0,len(line)):
        for i in range(1,len(line)-m):
            for k in range(0,len(line[m+i])):
                for n in range(0,len(line[m])):
                    point_coincides=coincide(line[m][n],line[m+i][k])
                    if point_coincides:
                        for point_coincide in point_coincides:
                            V[a]=point_coincide
                            a=a+1

                        x1, y1 = line[m][n].src.x, line[m][n].src.y
                        x2, y2 = line[m][n].dst.x, line[m][n].dst.y
                        x3, y3 = line[m+i][k].src.x, line[m+i][k].src.y
                        x4, y4 = line[m+i][k].dst.x, line[m+i][k].dst.y
                        V[a]=point(x1,y1)
                        V[a+1]=point(x2,y2)
                        V[a+2]=point(x3,y3)
                        V[a+3]=point(x4,y4)
                        a=a+4
                        E.append(Line(point(x1,y1),point(x2,y2)))
                        E.append(Line(point(x3,y3),point(x4,y4)))

                    else:
                        active=check_intersect(line[m][n],line[m+i][k])
                        if active:
                            intersection=intersect(line[m][n],line[m+i][k])
                            x1, y1 = line[m][n].src.x, line[m][n].src.y
                            x2, y2 = line[m][n].dst.x, line[m][n].dst.y
                            x3, y3 = line[m+i][k].src.x, line[m+i][k].src.y
                            x4, y4 = line[m+i][k].dst.x, line[m+i][k].dst.y
                            V[a]=point(x1,y1)
                            V[a+1]=point(x2,y2)
                            V[a+2]=point(x3,y3)
                            V[a+3]=point(x4,y4)
                            V[a+4]=intersection
                            intersects_set.append(intersection)
                            a=a+5
                            E.append(Line(point(x1,y1),point(x2,y2)))
                            E.append(Line(point(x3,y3),point(x4,y4)))


    return V,E,intersects_set

def final_vertex(Vs):
    V1=[]
    V={}
    a=0

    for e in Vs.values():
        V1.append(str(e))

    for i in set(V1):
        x1,y1=read_figure(i)
        V[a]=point(x1,y1)
        a=a+1
    return V

def possible_egde(V,p):
    edges={}
    m=0
    n=1
    for m in range(0,len(V)):
        for n in range(m+1,len(V)):
            x1,y1=read_figure(V[m])
            x2,y2=read_figure(V[n])
            edges['<{0},{1}>'.format(p[m],p[n])]=Line(point(x1,y1),point(x2,y2))
    return edges

def find_long_edge(Es):
    E1=[]
    E=[]
    for c in Es:
        E1.append(str(c))
    for q in set(E1):
        E.append(q)
    E1=[]
    for e in E:
        r=re.compile(r'\([-]?[0-9]+,[-]?[0-9]+\)')
        point_set=r.findall(e)
        x1,y1=read_figure(point_set[0])
        x2,y2=read_figure(point_set[1])
        E1.append(Line(point(x1,y1),point(x2,y2)))
    return E1


def final_edges(Es,edges,V,intersects_set):
    E1={}

    for name in edges.keys():
        x3, y3 = edges[name].src.x, edges[name].src.y
        x4, y4 = edges[name].dst.x, edges[name].dst.y
        for e in Es:
            s=0
            x1, y1 = e.src.x, e.src.y
            x2, y2 = e.dst.x, e.dst.y
            if x1==x3 and y1==y3 and x2==x4 and y2==y4:
                s=1
                for a in intersects_set:
                    xn=a.x
                    yn=a.y
                    if x1==xn and y1==yn:
                        s=0
                    elif x2==xn and y2==yn:
                        s=0
                    else:
                        pass
            elif x1==x4 and y1==y4 and x2==x3 and y2==y3:
                s=1
                for a in intersects_set:
                    xn=a.x
                    yn=a.y
                    if x1==xn and y1==yn:
                        s=0
                    elif x2==xn and y2==yn:
                        s=0
                    else:
                        pass
            if s==1:
                del edges[name]
                break

    for name in edges.keys():
        x3, y3 = edges[name].src.x, edges[name].src.y
        x4, y4 = edges[name].dst.x, edges[name].dst.y
        for e in Es:
            x1, y1 = e.src.x, e.src.y
            x2, y2 = e.dst.x, e.dst.y

            if abs(((x4-x3)*(y2-y1))-((y4-y3)*(x2-x1)))<0.1:
                if abs((x3-x1)*(y3-y2)-(y3-y1)*(x3-x2))<0.1:
                    if x3<=max(x1,x2) and x3>=min(x1,x2) and y3<=max(y1,y2) and y3>=min(y1,y2):
                        if x4<=max(x1,x2) and x4>=min(x1,x2) and y4<=max(y1,y2) and y4>=min(y1,y2):
                            E1[name]=Line(point(x3,y3),point(x4,y4))

    for name in E1.keys():
        x3, y3 = E1[name].src.x, E1[name].src.y
        x4, y4 = E1[name].dst.x, E1[name].dst.y
        for v in V:
            s=0
            x0,y0=read_figure(v)
            x0=float(x0)
            y0=float(y0)
            if abs((y0-y3)*(x0-x4)-(y0-y4)*(x0-x3))<0.1:
                if x0<=max(x3,x4) and x0>=min(x3,x4) and y0<=max(y3,y4) and y0>=min(y3,y4):

                    if x0==x3 and y0==y3:
                        s=0

                    elif x0==x4 and y0==y4:
                        s=0

                    else:
                        s=1

            if s==1:
                del E1[name]
                break
    return E1

def main():
    ### YOUR MAIN CODE GOES HERE
    input_point={}
    V={}
    E=[]
    while True:
        input_point,input_street,active,flag = run(input_point,sys.stdin, sys.stdout, sys.stderr)#a dictionary with street name and location
        if active=='1':
            break
        else:
            if flag!='wrong':
                if input_street[-1].startswith('g'):
                    for input_street_line in input_street[:-1]:
                        if input_street_line.startswith('a'):
                            line=input_point_line_set(input_point)#possible lines
                            Vs,Es,intersects_set=street_comparison(line)#final long lines with coincident lines and final points with coincident points
                            V=final_vertex(Vs)#a dictionary with final points


                        if input_street_line.startswith('c'):
                            line=input_point_line_set(input_point)#possible lines
                            Vs={}
                            Es=[]
                            Vc_str={}
                            V_final={}
                            Vc,Ec,intersects_set=street_comparison(line)#final long lines with coincident lines and final points with coincident points
                            Vc=final_vertex(Vc)#a dictionary with final points
                            for key in Vc.keys():
                                Vc_str[key]=str(Vc[key])

                            a=1
                            for key in V.keys():
                                for keyy in Vc_str.keys():
                                    if str(V[key])==Vc_str[keyy]:
                                        V_final[key]=V[key]
                                        a=a+1
                                        del Vc_str[keyy]
                                        break

                            if Vc_str!=[]:
                                for keyyy in Vc_str.keys():
                                    V_final[a]=Vc_str[keyyy]
                                    a=a+1

                            V=V_final
                            Es=Ec

                        if input_street_line.startswith('r'):
                            line=input_point_line_set(input_point)#possible lines
                            Vs={}
                            Es=[]
                            Vc_str={}
                            V_final={}
                            Vc,Ec,intersects_set=street_comparison(line)#final long lines with coincident lines and final points with coincident points
                            Vc=final_vertex(Vc)#a dictionary with final points
                            for key in Vc.keys():
                                Vc_str[key]=str(Vc[key])

                            a=1
                            for key in V.keys():
                                for keyy in Vc_str.keys():
                                    if str(V[key])==Vc_str[keyy]:
                                        V_final[key]=V[key]
                                        a=a+1
                                        del Vc_str[keyy]
                                        break

                            if Vc_str!=[]:
                                for keyyy in Vc_str.keys():
                                    V_final[a]=Vc_str[keyyy]
                                    a=a+1

                            V=V_final
                            Es=Ec






                V1=[]
                p=[]
                for key,value in V.items():
                    V1.append(str(value))
                    p.append(key)

                edges=possible_egde(V1,p)#line between every point
                E1=find_long_edge(Es)#final long lines
                E_final=final_edges(E1,edges,V1,intersects_set)#final lines
                E=[]
                for name in E_final.keys():
                    E.append(name)

                sys.stdout.write('V ')
                i=0
                for key,value in V.items():
                    i=i+1
                sys.stdout.write(str(i)+'\n')
                sys.stdout.write('E {')
                for name in E[:-1]:
                    sys.stdout.write(str(name)+',')
                if E:
                    sys.stdout.write(E[-1])
                sys.stdout.write('}\n')
                sys.stdout.flush();

if __name__ == '__main__':
    main()
