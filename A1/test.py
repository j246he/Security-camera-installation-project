## A simple unit test example. Replace by your own tests

import sys
import unittest
import re
from a1ece650 import street_info,read_point,coincide,read_figure,check_intersect
from a1ece650 import intersect,input_point_line_set,final_vertex
from a1ece650 import find_long_edge,final_edges,possible_egde,street_comparison
from a1ece650 import point,Line

class NamesTestCase(unittest.TestCase):
	
	def test_read_point(self):
		self.input_point=read_point('a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)')
		self.assertIn('(2,-1)',self.input_point)
		
	def test_read_figure(self):
		self.e='(0,2)'
		self.x,self.y=read_figure(self.e)
		self.assertEqual(self.x,'0')
		self.assertEqual(self.y,'2')
		
	def test_point(self):
		self.e='(0,2)'
		self.x,self.y=read_figure(self.e)
		self.point_test=point(self.x,self.y)
		self.assertEqual(str(self.point_test),'(0,2)')
		
	def test_Line(self):
		self.line=Line(point(2,-1),point(2,2))
		self.assertEqual(str(self.line),'[(2,-1)-->(2,2)]')
		
	def test_check_intersect(self):
		self.line1=Line(point(4,2),point(4,8))
		self.line2=Line(point(1,4),point(5,8))
		self.assertTrue(check_intersect(self.line1,self.line2))
	
	def test_coincide(self):
		self.e='[(0,2)-->(0,6)],[(0,1)-->(0,5)]'
		r=re.compile(r'\([-]?[0-9]+,[-]?[0-9]+\)')
		self.point_set=r.findall(self.e)
		self.x1,self.y1=read_figure(self.point_set[0])
		self.x2,self.y2=read_figure(self.point_set[1])
		self.x3,self.y3=read_figure(self.point_set[2])
		self.x4,self.y4=read_figure(self.point_set[3])
		self.line1=Line(point(self.x1,self.y1),point(self.x2,self.y2))
		self.line2=Line(point(self.x3,self.y3),point(self.x4,self.y4))
		self.point_final=coincide(self.line1,self.line2)
		self.assertEqual('[(0,5), (0,2)]'.strip(),str(self.point_final))
	
	def test_intersect(self):
		self.l1=Line(point(1,4),point(5,8))
		self.l2=Line(point(3,8),point(5,6))
		intersaction=intersect(self.l1,self.l2)
		self.assertEqual(str(intersaction),str(point(4,7)))
	
	def test_street_info(self):
		input_point={}
		self.input_point=street_info('a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)',input_point)
		self.assertIn('"Weber Street"',str(self.input_point.keys()))
		self.assertIn(['(2,-1)', '(2,2)', '(5,5)', '(5,6)', '(3,8)'],self.input_point.values())
	
	def test_input_point_line_set(self):
		self.input_point={'Weber Street ': ['(2,-1)', '(2,2)', '(5,5)', '(5,6)', '(3,8)'], 
							'King Street ': ['(4,2)', '(4,8)'], 
							'Davenport Road ': ['(1,4)', '(5,8)']}
		self.line=input_point_line_set(self.input_point)
		self.assertEqual('[(4,2)-->(4,8)]',str(self.line[1][0]))
	
	def final_vertex(self):
		self.Vs={1: (5,6), 2: (3,8), 3: (1,4), 4: (5,8), 5: (4,7), 
				6: (2,2), 7: (5,5), 8: (4,2), 9: (4,8), 10: (4,4), 
				11: (5,6), 12: (3,8), 13: (4,2), 14: (4,8), 15: (4,7), 
				16: (1,4), 17: (5,8), 18: (4,2), 19: (4,8), 20: (4,7)} 
		self.V_test={1: (2,2), 2: (4,7), 3: (5,8), 4: (4,4), 5: (5,6),
					6: (4,8), 7: (5,5), 8: (3,8), 9: (4,2), 10: (1,4)}
		self.V=final_vertex(self.Vs)
		self.assertEqual(self.V,self.Vs)
		
	def test_possible_egde(self):
		self.V=['(2,2)', '(4,7)', '(5,8)', '(4,4)', '(5,6)', '(4,8)',
				'(5,5)', '(3,8)', '(4,2)', '(1,4)']
		self.p=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
		self.edges=possible_egde(self.V,self.p)
		self.assertIn('[(4,4)-->(3,8)]',str(self.edges['<4,8>']))
	
	
	
	
if __name__ == '__main__':	
	unittest.main()
