namespace glosa{
namespace level_21_22{

intersect_result intersect(const line2d& line, const rect2d& rect, point2d* pt1, point2d* pt2, int* n_intersection){
	vector<point2d> pts;
	pts.reserve(2);
	
	//����ֱ�ߺ;����ıߵĽ�����
	for(int i = 0; i < 4; ++i){
		point2d line_edge_pt;
		intersect_result line_edge_test = intersect( line, rect.edge(i), &line_edge_pt );
		
		//���ֱ�ߺ�ĳ�߹����򷵻�Touches
		if(line_edge_test == intersect_result::contains){
			if( n_intersection ) { *n_intersection = 2; }
			if( *pt1 ) { *pt1 = edge.start; }
			if( *pt2 ) { *pt2 = edge.end; }
			return intersect_result::touches;
		}
		
		//���ֱ�Ӿ����߶εĶ˵㣬����ܲ���һ�����㣬�����޽��㡣
		if(line_edge_test == intersect_result::touches){
			pts.push_back(line_edge_pt);
			for(int j = 0; j < pts.size() - 1; ++j){
				if(pts[j] == line_edge_pt){
					pts.pop_back();
					break;
				}
			}
		}
		
		//���ֱ�ߴ���ĳ���߶Σ���ô�����һ������
		if( line_edge_test == intersection_result::crosses){
			pts.push_back(line_edge_pt);
		}
	}
	
	//���ݽ����������ཻ��Ϣ
	if( n_intersection ) { *n_intersection = 0; }
	
	if( pts.size() == 0){
		return intersect_result::disjoints;
	}
	
	if(pts.size() == 1){
		if( pt1 ) {* pt1 = pts[0];}
		return intersect_result::touches;
	}
	
	if( pt1 ) { *pt1 = pts[0]; }
	if( pt2 ) { *pt2 = pts[1]; }
	return intersect_result::crosses;
}

intersect_result intersect(const line2d& line, const oriented_rect2d& rect, point2d* pt1, point2d* pt2, int* n_intersection){
	vector<point2d> pts;
	pts.reserve(2);
	
	//����ֱ�ߺ;����ıߵĽ�����
	for(int i = 0; i < 4; ++i){
		point2d line_edge_pt;
		intersect_result line_edge_test = intersect( line, rect.edge(i), &line_edge_pt );
		
		//���ֱ�ߺ�ĳ�߹����򷵻�Touches
		if(line_edge_test == intersect_result::contains){
			if( n_intersection ) { *n_intersection = 2; }
			if( *pt1 ) { *pt1 = edge.start; }
			if( *pt2 ) { *pt2 = edge.end; }
			return intersect_result::touches;
		}
		
		//���ֱ�Ӿ����߶εĶ˵㣬����ܲ���һ�����㣬�����޽��㡣
		if(line_edge_test == intersect_result::touches){
			pts.push_back(line_edge_pt);
			for(int j = 0; j < pts.size() - 1; ++j){
				if(pts[j] == line_edge_pt){
					pts.pop_back();
					break;
				}
			}
		}
		
		//���ֱ�ߴ���ĳ���߶Σ���ô�����һ������
		if( line_edge_test == intersection_result::crosses){
			pts.push_back(line_edge_pt);
		}
	}
	
	//���ݽ����������ཻ��Ϣ
	if( n_intersection ) { *n_intersection = 0; }
	
	if( pts.size() == 0){
		return intersect_result::disjoints;
	}
	
	if(pts.size() == 1){
		if( pt1 ) {* pt1 = pts[0];}
		return intersect_result::touches;
	}
	
	if( pt1 ) { *pt1 = pts[0]; }
	if( pt2 ) { *pt2 = pts[1]; }
	return intersect_result::crosses;
}

intersect_result intersect(const line2d& line, const triangle2d& tri, point2d* pt1, point2d* pt2, int* n_intersection){
	vector<point2d> pts;
	pts.reserve(2);
	
	//����ֱ�ߺͱ߿�Ľ�����
	for(int i = 0; i < 3; ++i){
		point2d line_edge_pt;
		intersect_result line_edge_test = intersect( line, tri.edge(i), &line_edge_pt );
		
		//���ֱ�ߺ�ĳ�߹����򷵻�Touches
		if(line_edge_test == intersect_result::contains){
			if( n_intersection ) { *n_intersection = 2; }
			if( *pt1 ) { *pt1 = edge.start; }
			if( *pt2 ) { *pt2 = edge.end; }
			return intersect_result::touches;
		}
		
		//���ֱ�Ӿ����߶εĶ˵㣬����ܲ���һ�����㣬�����޽��㡣
		if(line_edge_test == intersect_result::touches){
			pts.push_back(line_edge_pt);
			for(int j = 0; j < pts.size() - 1; ++j){
				if(pts[j] == line_edge_pt){
					pts.pop_back();
					break;
				}
			}
		}
		
		//���ֱ�ߴ���ĳ���߶Σ���ô�����һ������
		if( line_edge_test == intersection_result::crosses){
			pts.push_back(line_edge_pt);
		}
	}
	
	//���ݽ����������ཻ��Ϣ
	if( n_intersection ) { *n_intersection = 0; }
	
	if( pts.size() == 0){
		return intersect_result::disjoints;
	}
	
	if(pts.size() == 1){
		if( pt1 ) {* pt1 = pts[0];}
		return intersect_result::touches;
	}
	
	if( pt1 ) { *pt1 = pts[0]; }
	if( pt2 ) { *pt2 = pts[1]; }
	return intersect_result::crosses;
}

intersect_result intersect(const ray2d& ray, const rect2d& rect, point2d* pt1, point2d* pt2, int* n_intersection){
	line2d ray_line(ray);
	point2d pts[2];
	int intersection_count;
	
	//ֱ��-���β���
	intersect_result rayline_rect_test = intersect( line, rect, &pts[0], &pts[1], &intersection_count);
	
	//���ֱ�߾��β��ཻ������Ҳ���ཻ��
	if( rayline_rect_test == intersect_result::disjoints ){
		if( n_intersection ) { *n_intersection == 0; }
		return intersect_result::disjoints;
	}
	
	//���ֱ����������
	if( rayline_rect_test == intersect_result::touches ){
		
		//һ�������ʱ��
		if( intersect_count == 1 ){
			if( ray.t(pts[0]) >= (GLOSA_FLOAT)0 ){
				if( pt1 ) { *pt1 = pts[0]; }
				if( n_intersection ) { *n_intersection = 1; }
				return intersect_result::touches;
			}
			
			if( n_intersection ) { *n_intersection = 0; }
			return intersect_result::disjoints;
		}
		
		//���������ʱ��
		GLOSA_FLOAT	 t_pt0_on_ray = ray.t(pts[0]);
		GLOSA_FLOAT	 t_pt1_on_ray = ray.t(pts[1]);
		GLOSA_FLOAT	min_t = min(t_pt0_on_ray, t_pt1_on_ray);
		GLOSA_FLOAT max_t = max(t_pt0_on_ray, t_pt1_on_ray);
		
		if( equal( max_t , (GLOSA_FLOAT)0.0 ){
			if( n_intersection ) { *n_intersection = 0; }
			if( pt1 ) {*pt1 = ray.start; }
			return intersect_result::touches;
		}
		
		if( max_t < 0 ){
			if( n_intersection ) { *n_intersection = 0; }
			return intersect_result::disjoints;
		}
		
		//max_t > 0
		if( n_intersection ) { *n_intersection = 2; }
		if(min_t > 0){
			if( pt1 ) { *pt1 = pts[0]; }
			if( pt2 ) { *pt2 = pts[1]; }
		} else {
			//max_t > 0 && min_t <= 0
			if( pt1 ) { *pt1 = pts[t_pt0_on_ray > 0 ? 0 : 1]; }
			if( pt2 ) { *pt2 = ray.start; }
		}
		return intersect_result::touches;
	}
	
	if( rayline_rect_test == intersect_result::crosses){
		if( intersect_count == 1 ){
			if( pts[0] == ray.start ){
				if( pt1 ) { *pt1 = pts[0]; }
				if( n_intersection ) { *n_intersection = 1; }
				return intersect_result::touches;
			}
			if( ray.t(pts[0]) = (GLOSA_FLOAT)0 ){
				if( pt1 ) { *pt1 = pts[0]; }
				if( n_intersection ) { *n_intersection = 1; }
				return intersect_result::crosses;
			}
			
			if( n_intersection ) { *n_intersection = 0; }
			return intersect_result::disjoints;
		}
	
		GLOSA_FLOAT	 t_pt0_on_ray = ray.t(pts[0]);
		GLOSA_FLOAT	 t_pt1_on_ray = ray.t(pts[1]);
		GLOSA_FLOAT	min_t = min(t_pt0_on_ray, t_pt1_on_ray);
		GLOSA_FLOAT max_t = max(t_pt0_on_ray, t_pt1_on_ray);
		
		if( equal( max_t , (GLOSA_FLOAT)0.0 ){
			if( n_intersection ) { *n_intersection = 0; }
			if( pt1 ) {*pt1 = ray.start; }
			return intersect_result::touches;
		}
		
		if( max_t < 0 ){
			if( n_intersection ) { *n_intersection = 0; }
			return intersect_result::disjoints;
		}
		
		//max_t > 0
		if( n_intersection ) { *n_intersection = 2; }
		if(min_t > 0){
			if( pt1 ) { *pt1 = pts[0]; }
			if( pt2 ) { *pt2 = pts[1]; }
			return intersect_result::crosses;
		} 
		
		if( equal( min_t, (GLOSA_FLOAT)0 ) ){
			if( pt1 ) { *pt1 = pts[t_pt0_on_ray > 0 ? 0 : 1]; }
			if( pt2 ) { *pt2 = ray.start; }
			if( n_intersection ) { *n_intersection = 2; }
			return intersect_result::crosses;
		}
		
		//max_t > 0 && min_t <= 0
		if( pt1 ) { *pt1 = pts[t_pt0_on_ray > 0 ? 0 : 1]; }
		if( n_intersection ) { *n_intersection = 1; }
		return intersect_result::crosses;
	}
}

intersect_result intersect(const ray2d& ray, const oriented_rect2d* rect, point2d* pt1, point2d* pt2, int* n_intersection){
}

intersect_result intersect(const ray2d& ray, const triangle2d& tri, point2d* pt1, point2d* pt2, int* n_intersection){
}

intersect_result intersect(const segment2d& seg, const rect2d& rect, point2d* pt1, point2d* pt2, int* n_intersection){
}

intersect_result intersect(const segment2d& seg, const oriented_rect2d* rect, point2d* pt1, point2d* pt2, int* n_intersection){
}

intersect_result intersect(const segment2d& seg, const triangle2d& tri, point2d* pt1, point2d* pt2, int* n_intersection){
}

}
}