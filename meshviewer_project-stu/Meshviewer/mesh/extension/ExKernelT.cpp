
#include "ExKernelT.h"
#include<math.h>
#include <map>

namespace MeshN { 

	///////////////////////////////////////////////////////////////////////////////
	// Implementation of member functions of ExKernelT
	/////////////////////////////////////////////////////////////////////////////// 
	template<class ExItems>
	ExKernelT<ExItems>::ExKernelT(): KernelT<ExItems>() {

		//kdtree_ = NULL;
		//ps_     = NULL;
		isNormal_ = false;
		isArea_ = false;

	}
	//////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	ExKernelT<ExItems>::~ExKernelT(){

		//if(kdtree_ != NULL) delete kdtree_;

		//if(ps_ != NULL)     delete ps_;

	}
	//////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	void ExKernelT<ExItems>::meshInit(){

		update_facet_normals();//��������������Ƭ�ķ���
		VertexIterator vit(vertex_begin());

		/*for (; vit != vertex_end(); vit++) {

		VertexHandle vh = vertex_handle(vit->halfedge_handle_);
			fprintf(fp, " %f  %f  %f\n", coord(vh).data_[0], coord(vh).data_[1], coord(vh).data_[2]);

		}*/
		//calc_normal(vh);
		update_area();
		update_edge_length();//������������ı߳�


	}

	///////////////////////////////////////////////////////////////////////////////

	template<class ExItems>
	void ExKernelT<ExItems>::Laplacian_Smoothing() {

		/////��ʵ���Լ���ȥ���㷨////////
		int n;
		std::cout << "�������������:";
		std::cin >> n;
		double laplac = 0.0;
		std::cout << "������������˹ϵ��(Խ�ӽ�����->1):";
		std::cin >> laplac;
		for (int i = 1; i <= n; i++)
		{
			std::vector<Coord> updateVertexPosition;
			std::vector<VertexHandle> v_near;
			int vertex_num = vertex_size();
			updateVertexPosition.resize(vertex_num);
			for (int i = 0; i < vertex_num; i++) {

				VertexHandle vh(i);
				getNeighborRing(vh, 1, v_near);
				double xsum = 0.0, ysum = 0.0, zsum = 0.0, dsum = 0.0;
				double d = 0.0;
				for (int j = 1; j < v_near.size(); j++)
				{
					d = sqrt((coord(v_near[j]).x- coord(v_near[0]).x) * (coord(v_near[j]).x - coord(v_near[0]).x) + (coord(v_near[j]).y - coord(v_near[0]).y) * (coord(v_near[j]).y - coord(v_near[0]).y) + (coord(v_near[j]).z - coord(v_near[0]).z) * (coord(v_near[j]).z - coord(v_near[0]).z));
					dsum += d;
					xsum += coord(v_near[j]).x*d;
					ysum += coord(v_near[j]).y*d;
					zsum += coord(v_near[j]).z*d;
				}
				xsum = xsum / dsum;
				ysum = ysum / dsum;
				zsum = zsum / dsum;
				Coord cd(xsum, ysum, zsum);
				updateVertexPosition[i] = coord(v_near[0]) * laplac + cd * (1 - laplac);
				v_near.clear();
			}
			for (int i = 0; i < vertex_num; i++)
			{
				vertex_ref(VertexHandle(i)).coord_ = updateVertexPosition[i];
			}
		}
	}

////////////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	void ExKernelT<ExItems>::mesh_process(std::map<VertexHandle,double> &Gosk){///////(��ѡ��������������ʵ��һ�ֲ���������������ȡ������ָ����������εȵ�)
		VertexIterator vi = vertex_begin();
		for (; vi != vertex_end(); ++vi) {
			if (vi->status_.is_deleted()) continue;

			assert(vi->halfedge_handle_.is_valid());
			VertexHandle& vh = vertex_handle(vi->halfedge_handle_);
			std::vector<FacetHandle> _fhs;
			HalfedgeHandle& hh = halfedge_handle(vh);
			VertexHandle  vhs;
			vhs = vertex_handle(hh);
			FacetHandle _fh = facet_handle(hh);
			HalfedgeHandle& hhv = halfedge_handle(vhs);
			HalfedgeHandle cursor(hhv);
			FacetHandle fh = facet_handle(cursor);
			_fhs.push_back(_fh);//�����һ��
			int j;
			do {
				FacetHandle fh = facet_handle(cursor);
				if (fh.is_valid() && fh != _fh) {

					if (_fhs.size() != 0) {

						for (j = 0; j < _fhs.size(); j++) {

							if (fh.idx() == _fhs[j].idx()) break;
						}//end for

						if (j >= _fhs.size()) _fhs.push_back(fh);

					}//end if
					else _fhs.push_back(fh);
				}//end if

				cursor = cw_rotated(cursor);

			} while (hhv != cursor);//end for do while
			double total_area = 0.0;
			double area = 0.0;
			double Angle = 2 * 3.1415926535898;
			for (int i = 0; i < _fhs.size(); i++) {
				const HalfedgeHandle& hh = halfedge_handle(_fhs[i]);
				const HalfedgeHandle& p_hh = prev_halfedge_handle(hh);
				const HalfedgeHandle& n_hh = next_halfedge_handle(hh);
				Coord cd0 = coord(vertex_handle(hh));
				Coord cd1 = coord(vertex_handle(p_hh));
				Coord cd2 = coord(vertex_handle(n_hh));
				Coord cd_ori = coord(vh);
				if (cd_ori == cd0) {
					;
				}
				else if (cd_ori == cd1) {
					cd1 = coord(vertex_handle(hh));
					cd0 = coord(vh);
				}
				else if (cd_ori == cd2) {
					cd2 = coord(vertex_handle(hh));
					cd0 = coord(vh);
				}
				double a = sqrt(pow(cd0.data_[0] - cd1.data_[0], 2) + pow(cd0.data_[1] - cd1.data_[1], 2) + pow(cd0.data_[2] - cd1.data_[2], 2));
				double b = sqrt(pow(cd0.data_[0] - cd2.data_[0], 2) + pow(cd0.data_[1] - cd2.data_[1], 2) + pow(cd0.data_[2] - cd2.data_[2], 2));
				double c = sqrt(pow(cd2.data_[0] - cd1.data_[0], 2) + pow(cd2.data_[1] - cd1.data_[1], 2) + pow(cd2.data_[2] - cd1.data_[2], 2));
				int tri = 0;//0-���,1-�۽�,2-ֱ��
				if (a * a + b * b > c * c && a * a + c * c > b * b && b * b + c * c > a * a) {
					tri = 0;
				}
				else if (a * a + b * b == c * c || a * a + c * c == b * b || b * b + c * c == a * a) {
					tri = 2;
				}
				else {
					tri = 1;
				}
				double angle_ori = acos((a * a + b * b - c * c) / (2 * a * b));
				Angle -= angle_ori;
				double anglej = acos((a * a + c * c - b * b) / (2 * a * c));
				double anglej1 = acos((b * b + c * c - a * a) / (2 * b * c));
				if (tri == 0) {
					double temp = 1 / tan(anglej);
					double temp2 = 1 / tan(anglej1);
					area = (0.125) * (b*b*temp+c*c*temp2);
				}
				else if (tri == 1) {
					area = (calc_facet_area(_fhs[i])) - (1 / 8) * (c * c * tan(anglej)) - (1 / 8) * (a * a * tan(anglej1));
				}
				else if (tri == 2) {
					area = (1 / 8) * ((b * b) * (1 / tan(anglej)) + (c * c) * (1 / tan(anglej1)));
				}
				total_area += area;
			}
			double GossK = Angle / total_area;
			Gosk.insert(std::pair<VertexHandle,double>{ vh,GossK });
		}
	}

////////////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	typename ExKernelT<ExItems>::Scalar
		ExKernelT<ExItems>::calc_facet_area(const FacetHandle& _fh){/////���������ε����

			///�õ���߾��
			assert(_fh.is_valid());
			assert(_fh.idx() < facet_size());

			const HalfedgeHandle& hh = halfedge_handle(_fh);
			const HalfedgeHandle& p_hh = prev_halfedge_handle(hh);
			const HalfedgeHandle& n_hh = next_halfedge_handle(hh);

			////�ɰ�߾���õ��߾��
			const Coord& cd0 = coord(vertex_handle(hh));
			const Coord& cd1 = coord(vertex_handle(p_hh));
			const Coord& cd2 = coord(vertex_handle(n_hh));
			double length0 = sqrt(pow(cd0.data_[0]-cd1.data_[0],2)+pow( cd0.data_[1] - cd1.data_[1], 2)+ pow(cd0.data_[2] - cd1.data_[2], 2));
			double length1 = sqrt(pow(cd0.data_[0] - cd2.data_[0], 2) + pow(cd0.data_[1] - cd2.data_[1], 2) + pow(cd0.data_[2] - cd2.data_[2], 2));
			double length2 = sqrt(pow(cd2.data_[0] - cd1.data_[0], 2) + pow(cd2.data_[1] - cd1.data_[1], 2) + pow(cd2.data_[2] - cd1.data_[2], 2));
			////�ɱ߾���õ����߳�
			double p = (length0 + length1 + length2) / 2;

			////���ú��׹�ʽ�����s=sqrt(p(p-a)(p-b)(p-c))
			Scalar area = 0.0;
			area = sqrt(p * (p - length0) * (p - length1) * (p - length2));


			facet_ref(_fh).area_ = area;
			return area;
	}

	//////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	void ExKernelT<ExItems>::update_area()//��������������Ƭ�����
	{   
		set_isArea(true);
		Scalar max_area = 0.0;

		FacetIterator fit(facet_begin() );
		for(;fit<facet_end(); fit++)
		{
			HalfedgeHandle hh = fit->halfedge_handle_;
			FacetHandle    fh = facet_handle(hh);
			Scalar curr_area = calc_facet_area(fh);

			if (curr_area > max_area)
				max_area = curr_area;
		}

		std::cout << "The maximal area of the mesh is: " << max_area << std::endl;
	}
	///////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	typename ExKernelT<ExItems>::Scalar
		ExKernelT<ExItems>::get_area(const FacetHandle& _fh)
	{
		return facet_ref(_fh).area_;
	}
	//////////////////////////////////////////////////////////////////////////////
	template<class ExItems> 
	double 
		ExKernelT<ExItems>::calc_edge_length(const EdgeHandle &_eh) {//updating an edge length


			HalfedgeHandle& h1 = halfedge_handle(_eh,0);
			HalfedgeHandle& h2 = halfedge_handle(_eh,1);

			Vertex v0 = vertex_ref(vertex_handle(h1) );
			Vertex v1 = vertex_ref(vertex_handle(h2) );

			return (v0.coord_-v1.coord_).norm();

	}

	template<class ExItems>
	void
		ExKernelT<ExItems>::getNeighborRing(VertexHandle& _vh, int _ring, std::vector<VertexHandle>& NeighborRing){//�õ���ring���ڽӵ�

			NeighborRing.push_back( _vh );
			int iteration = 0;
			int verNewNum = NeighborRing.size();
			int verOldNum = verNewNum-1;
			int verOldNum1 = verOldNum;

			do{
				verOldNum = NeighborRing.size();
				for(int i=verOldNum1; i<verNewNum; i++){
					VertexHandle& vh = NeighborRing[i];
					HalfedgeHandle& hh = halfedge_handle(vh);
					HalfedgeHandle css(hh);
					do{
						int ii = 0;
						HalfedgeHandle& opp_hh = opposite_halfedge_handle(css);
						VertexHandle&   opp_vh = vertex_handle(opp_hh);
						for(ii=0; ii<NeighborRing.size(); ii++)
							if(opp_vh == NeighborRing[ii] ) break;

						if(ii >= NeighborRing.size() )
							NeighborRing.push_back(opp_vh);

						css = cw_rotated(css);
					}while(css != hh);
				}

				verNewNum = NeighborRing.size();
				verOldNum1 = verOldNum;
				iteration++;
			}while(iteration < _ring);

	}
	///////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	void
		ExKernelT<ExItems>::getNeighborFaceN1(FacetHandle& _fh, std::vector<FacetHandle>& _fhs){//sharing common edges

			HalfedgeHandle& hh = halfedge_handle(_fh);
			HalfedgeHandle& pre_hh = prev_halfedge_handle(hh);
			HalfedgeHandle& nex_hh = next_halfedge_handle(hh);

			_fhs.push_back( facet_handle( opposite_halfedge_handle(hh) ) );
			_fhs.push_back( facet_handle( opposite_halfedge_handle(pre_hh ) ) );
			_fhs.push_back( facet_handle(opposite_halfedge_handle(nex_hh ) ) );
	}
	///////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	void 
		ExKernelT<ExItems>::getNeighborFaceN2(FacetHandle& _fh, std::vector<FacetHandle>& _fhs){//sharing common vertices

			HalfedgeHandle& hh = halfedge_handle(_fh);
			HalfedgeHandle& pre_hh = prev_halfedge_handle(hh);
			HalfedgeHandle& nex_hh = next_halfedge_handle(hh);

			VertexHandle  vhs[3];
			vhs[0] = vertex_handle(hh);
			vhs[1] = vertex_handle(pre_hh);
			vhs[2] = vertex_handle(nex_hh);
			int i = 0, j=0;

			for(i=0; i<3; i++){

				HalfedgeHandle& hhv = halfedge_handle( vhs[i] );
				HalfedgeHandle cursor(hhv);

				do{

					FacetHandle fh = facet_handle(cursor);
					if(fh.is_valid() && fh != _fh){

						if(_fhs.size() != 0){

							for(j=0; j< _fhs.size(); j++){

								if(fh.idx() == _fhs[j].idx() ) break;
							}//end for

							if(j>= _fhs.size() ) _fhs.push_back(fh);

						}//end if
						else _fhs.push_back(fh);
					}//end if

					cursor = cw_rotated(cursor);

				}while(hhv != cursor);//end for do while
			}//end for

	}
	///////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	void ExKernelT<ExItems>::output_to_file(){

		FILE *fp;
		fp=fopen("My_result.off","w");

		int no_vertex=vertex_size();
		int no_facet=facet_size();
		int edge = 0;

		fprintf(fp,"OFF\n");
		fprintf(fp,"%d  %d %d\n",no_vertex,no_facet, edge);

		VertexIterator vit(vertex_begin());

		for(;vit!=vertex_end();vit++){

			VertexHandle vh=vertex_handle(vit->halfedge_handle_);
			fprintf(fp," %f  %f  %f\n",coord(vh).data_[0],coord(vh).data_[1],coord(vh).data_[2]);

		}

		FacetIterator fit(facet_begin());

		for(;fit!=facet_end();fit++){

			HalfedgeHandle hh=fit->halfedge_handle_;
			HalfedgeHandle nh=next_halfedge_handle(hh);
			HalfedgeHandle nnh=next_halfedge_handle(nh);

			VertexHandle vh=vertex_handle(hh);
			VertexHandle nvh=vertex_handle(nh);
			VertexHandle nnvh=vertex_handle(nnh);

			fprintf(fp,"3 %d  %d  %d\n",vh.idx(),nvh.idx(),nnvh.idx());

		}

		fclose(fp);
	}
//////////////////////////////////////////////////////////////////////////////////
	template <class ExItems> 
	typename ExKernelT<ExItems>::Normal
		ExKernelT<ExItems>::normal(const FacetHandle& _fh) {
			assert( _fh.is_valid() );
			assert( _fh.idx() < facet_size() );
			return facet_ref(_fh).normal_;
	}


	///////////////////////////////////////////////////////////////////////////////
	template <class ExItems> 
	typename ExKernelT<ExItems>::Normal
		ExKernelT<ExItems>::calc_normal(const FacetHandle& _fh) {
			assert( _fh.is_valid() );
			assert( _fh.idx() < facet_size() );

			const HalfedgeHandle&   hh = halfedge_handle(_fh);
			const HalfedgeHandle& p_hh = prev_halfedge_handle(hh);
			const HalfedgeHandle& n_hh = next_halfedge_handle(hh);

			const Coord& cd0 = coord( vertex_handle( hh) );
			const Coord& cd1 = coord( vertex_handle(p_hh) );
			const Coord& cd2 = coord( vertex_handle(n_hh) );

			//return ((cd1-cd0)%(cd2-cd1)).normalize();
			return ((cd2-cd1)%(cd1-cd0)).normalize();//����������˲��ҵ�λ�� be careful
	}


	///////////////////////////////////////////////////////////////////////////////
	template <class ExItems> 
	void ExKernelT<ExItems>::update_facet_normals(void) {

		set_isNormal(true);
		FacetIterator fi = facet_begin();

		for ( ; fi!=facet_end(); ++fi) {
			if (fi->status_.is_deleted()) continue;

			assert(fi->halfedge_handle_.is_valid());
			fi->normal_ = calc_normal( facet_handle(fi->halfedge_handle_) );
		}
	}


	///////////////////////////////////////////////////////////////////////////////
	template <class ExItems> 
	typename ExKernelT<ExItems>::Normal
		ExKernelT<ExItems>::normal(const VertexHandle& _vh) {
			assert( _vh.is_valid() );
			assert( _vh.idx() < vertex_size() );
			return vertex_ref(_vh).normal_;
	}


///////////////////////////////////////////////////////////////////////////////
	template <class ExItems> 
	typename ExKernelT<ExItems>::Normal
		ExKernelT<ExItems>::calc_normal(const VertexHandle& _vh) {////����һ������ķ���
			assert( _vh.is_valid());
			assert( _vh.idx() < vertex_size() );

			Normal          norm(0,0,0);///////��norm�洢��õķ���ֵ

//////////////////////////�ڴ�ʵ��////////////////////////////////////
			std::vector<FacetHandle> _fhs;
			HalfedgeHandle& hh = halfedge_handle(_vh);
			VertexHandle  vhs;
			vhs = vertex_handle(hh);
			FacetHandle _fh = facet_handle(hh);
			HalfedgeHandle& hhv = halfedge_handle(vhs);
			HalfedgeHandle cursor(hhv);
			FacetHandle fh = facet_handle(cursor);
			_fhs.push_back(_fh);//�����һ��
			int j;
			do {
				FacetHandle fh = facet_handle(cursor);
				if (fh.is_valid() && fh != _fh) {

					if (_fhs.size() != 0) {

						for (j = 0; j < _fhs.size(); j++) {

							if (fh.idx() == _fhs[j].idx()) break;
						}//end for

						if (j >= _fhs.size()) _fhs.push_back(fh);

					}//end if
					else _fhs.push_back(fh);
				}//end if

				cursor = cw_rotated(cursor);

			} while (hhv != cursor);//end for do while
			double total_area = 0.0;
			for (int i = 0; i < _fhs.size(); i++)
			{
				total_area += calc_facet_area(_fhs[i]);
			}
			for (int i = 0; i < _fhs.size(); i++)
			{
				double weight = calc_facet_area(_fhs[i]) / total_area;
				norm += calc_normal(_fhs[i]) * weight;
			}

/////////////////////////////////////////////////////////////////////

			return norm.normalize();
	}


///////////////////////////////////////////////////////////////////////////////
	template <class ExItems> 
	void ExKernelT<ExItems>::update_vertex_normals(void) {//�������ж���ķ���
		VertexIterator vi = vertex_begin();

		for ( ; vi!=vertex_end(); ++vi) {
			if (vi->status_.is_deleted()) continue;

			assert(vi->halfedge_handle_.is_valid());
			vi->normal_ = calc_normal( vertex_handle(vi->halfedge_handle_) );
		}
	}

///////////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	typename ExKernelT<ExItems>::Normal
		ExKernelT<ExItems>::calc_normal_max(const VertexHandle& _vh){

			assert(_vh.is_valid() );
			assert(_vh.idx() < vertex_size() );

			HalfedgeHandle& hh = halfedge_handle(_vh);
			Coord& vc = vertex_ref(_vh).coord_;
			Normal n(0,0,0);
			HalfedgeHandle css(hh);
			do{
				FacetHandle& fh = facet_handle(css);
				if(fh.is_valid() ){

					HalfedgeHandle& nexhh = next_halfedge_handle(css);
					HalfedgeHandle& prehh = prev_halfedge_handle(css);

					VertexHandle& nvh = vertex_handle(nexhh);
					VertexHandle& prevh = vertex_handle(prehh);

					Coord& nvc = vertex_ref(nvh).coord_;
					Coord& prevc = vertex_ref(prevh).coord_;

					Coord vec1 = vc - nvc;
					Coord vec2 = vc - prevc;
					Coord vec12cross = vec1 % vec2;//cross multiplication

					float weight = vec12cross.length() / (vec1.sqLength() * vec2.sqLength() );

					n += facet_ref(fh).normal_ * weight;//---------------------------------------ע��
					//n += calc_normal(fh);//---------------------------------ע��

				}//if

				css = cw_rotated(css);
			}while(css != hh);

			n.normalize();

			return n;

	}
	//////////////////////////////////////////////////////////////////////////////////
	template <class ExItems> 
	void ExKernelT<ExItems>::update_vertex_normals_max(void){

		VertexIterator vi = vertex_begin();

		for ( ; vi!=vertex_end(); ++vi) {
			if (vi->status_.is_deleted() ) continue;

			assert(vi->halfedge_handle_.is_valid());
			//vi->normal_ = calc_normal( vertex_handle(vi->halfedge_handle_));
			vi->normal_ = calc_normal_max( vertex_handle(vi->halfedge_handle_) );
			//std::cout<<vi->normal_<<std::endl;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////
	template <class ExItems> 
	void ExKernelT<ExItems>::update_normals(void) {
		update_facet_normals();
		update_vertex_normals();
	}


	///////////////////////////////////////////////////////////////////////////////
	template <class ExItems> 
	void ExKernelT<ExItems>::update_edge_length(void) {//���������б߳���Ϣ
		float global_max_edge_length_ = 0;
		float averagedlength = 0.0;

		EdgeIterator eit = edge_begin();
		for ( ; eit!=edge_end(); ++eit ) {
			Vertex& v0 = vertex_ref( eit->halfedges_[0].vertex_handle_ );
			Vertex& v1 = vertex_ref( eit->halfedges_[1].vertex_handle_ );

			eit->length_ = (v0.coord_ - v1.coord_).norm();
			//std::cout<<eit->length_<<"\n";
			averagedlength += eit->length_;

			if (global_max_edge_length_ < eit->length_)
				global_max_edge_length_ = eit->length_; 
		} 

		averagedlength /= edge_size();
		set_average_edge_length(averagedlength);
	}
	/////////////////////////////////////////////////////////////////////////
	template<class ExItems>
	typename ExKernelT<ExItems>::Coord
		ExKernelT<ExItems>::calc_centroid(const FacetHandle& _fh){

			HalfedgeHandle& hh = halfedge_handle(_fh);
			HalfedgeHandle& n_hh = next_halfedge_handle(hh);
			HalfedgeHandle& pre_hh = prev_halfedge_handle(hh);

			VertexHandle& vh = vertex_handle(hh);
			VertexHandle& n_vh = vertex_handle(n_hh);
			VertexHandle& pre_vh = vertex_handle(pre_hh);

			return Coord(coord(vh) +
				coord(n_vh)+
				coord(pre_vh) )/3.0;

	}

	//////////////////////////////////////////////////////////////////////////
	//template<class ExItems>
	//bool ExKernelT<ExItems>::createPS(){

	//	unsigned int NumFacet = facet_size();

	//	ps_ = new PointSet(NumFacet);//��ʼ��

	//	FacetIterator fi = facet_begin();
	//	unsigned int i = 0;

	//	for(; fi != facet_end(); fi++ ){

	//		Coord& cor = calc_centroid( facet_handle(fi->halfedge_handle_)
	//			);
	//		ps_->setPoint(i, cor[0], cor[1], cor[2] );

	//		i++;
	//	}

	//	//std::cout<<ps_->pointN<<"pointN"<<std::endl;

	//	return true;

	//}
	///////////////////////////////////////////////////////////////
	//template<class ExItems>
	//bool ExKernelT<ExItems>::createKdTree(){

	//	kdtree_ = new KdTree(ps_);

	//	//std::cout<<kdtree_->listN<<"listN"<<std::endl;

	//	return true;


	//}
	
////////////////////////////////////////////////////////////////
	template<class ExItems>
	void
		ExKernelT<ExItems>::adjustFaceNormal_FE(){///Sun Xiangfang TVCG 2007 fast and effective feature-preserving mesh denoising

			int facet_num = facet_size();

			double T;
			std::cout<<"Input Threshold(0.0-1.0): ";
			std::cin>>T;

			int iteration;
			std::cout<<"Input facenormal filtering numbers (5-20��): ";
			std::cin>>iteration;
			std::cout << "���ڵ������ȽϺ�ʱ��"<<std::endl;
			std::cout << "Please wait.....  "<<std::endl;
			int i = 0, j = 0;
			clock_t t1 = clock();
			do{

				std::vector<Normal> updateFacetNormal;
				updateFacetNormal.resize(facet_num);

				FacetIterator fit = facet_begin();
				for( ; fit!=facet_end(); fit++){
					std::vector<FacetHandle> fhs;
					HalfedgeHandle hh = fit->halfedge_handle_;
					FacetHandle    fh = facet_handle(hh);
					fhs.push_back(fh);
					//VertexHandle varry[3];
					std::vector<VertexHandle> varry;
					varry.resize(3);
					varry[0] = vertex_handle(hh);
					varry[1] = vertex_handle( prev_halfedge_handle(hh) );
					varry[2] = vertex_handle( next_halfedge_handle(hh) );
					for(i=0; i<3; i++){
						HalfedgeHandle vahh = halfedge_handle(varry[i] );
						HalfedgeHandle vacss(vahh);
						do{
							FacetHandle vafh = facet_handle(vacss );
							if(vafh.is_valid() ){

								for(j=0;j<fhs.size(); j++)
									if(vafh == fhs[j] ) break;
								if(j>=fhs.size() ) fhs.push_back(vafh);
							}

							vacss = cw_rotated(vacss );

						}while(vacss != vahh);
					}

					Normal& nf = facet_ref(fh).normal_;
					for(i=0; i<fhs.size(); i++){

						Normal& nfi = facet_ref(fhs[i]).normal_;
						double dotproduct = nf*nfi - T;
						//double dotproduct = 1 - nf*nfi;
						if(dotproduct <1e-7) continue;
						//dotproduct = exp(dotproduct);

						dotproduct *= dotproduct;

						updateFacetNormal[fh.idx()] += nfi * dotproduct;


					}


				}

				for(fit=facet_begin();fit != facet_end(); fit++){
					facet_ref(facet_handle(fit->halfedge_handle_ ) ).normal_ = updateFacetNormal[facet_handle(fit->halfedge_handle_).idx()].normalize();
				}

			}while(--iteration);

			clock_t t2 = clock();

			//t2 = (t2-t1)*1.0/CLOCKS_PER_SEC;
			std::cout<<"The time of Sun's normal filter: "<<(t2-t1)*1.0/CLOCKS_PER_SEC<<"s"<<std::endl;

	}
	////////////////////////////////////////////////////////////////
	template<class ExItems>
	void 
		ExKernelT<ExItems>::Mesh_Denoising_FE(){//Sun Xianfang TVCG2007
			adjustFaceNormal_FE();
			//adjustFaceNormal_Based_on_FE();

			int vertex_num = vertex_size();
			int iterations;
			std::cout<<"Input vertex update iterations (10-30��): ";
			std::cin>>iterations;
			std::cout << "���ڵ������ȽϺ�ʱ��" << std::endl;
			std::cout << "Please wait.....  " << std::endl;
			int i = 0;

			clock_t t1 = clock();
			do{
				std::vector<Coord> updateVertexPosition;
				updateVertexPosition.resize(vertex_num);
				for(i=0; i<vertex_num; i++){
					VertexHandle vh(i);
					Coord&       vc = coord(vh);
					HalfedgeHandle& hh = halfedge_handle(vh);
					HalfedgeHandle  css(hh);
					do{
						HalfedgeHandle opp_hh = opposite_halfedge_handle(css);
						Coord&         opp_vc = coord(vertex_handle(opp_hh) );
						FacetHandle    fl = facet_handle(css);
						FacetHandle    fr = facet_handle(opp_hh);

						if(fl.is_valid() ){
							updateVertexPosition[i] += facet_ref(fl).normal_*( facet_ref(fl).normal_ *(opp_vc - vc) );

						}
						if(fr.is_valid() ){
							updateVertexPosition[i] += facet_ref(fr).normal_*( facet_ref(fr).normal_ *(opp_vc - vc) );

						}

						css = cw_rotated(css);

					}while(css != hh);
				}
				for(i=0; i<vertex_num; i++){ vertex_ref(VertexHandle(i) ).coord_ += updateVertexPosition[i]*1.0/18.0;}

			}while(--iterations);

			clock_t t2 = clock();
			//t2 = (t2-t1)/CLOCKS_PER_SEC;
			std::cout<<"The time of Sun's vertex updating: "<<(t2-t1)*1.0/CLOCKS_PER_SEC<<"s"<<std::endl;

	}
	////////////////////////////////////////////////
	template<class ExItems>
	void 
		ExKernelT<ExItems>::adjustFaceNormal_YouyiZheng(){

			double SigmaC = get_average_edge_length();
			float MinSigmaS, MaxSigmaS;
			//calcdSigmaS1(MinSigmaS,MaxSigmaS);
			//MaxSigmaS /= 2.0;
			std::cout<<"Please input SigmaS (0.2-0.6): ";
			std::cin>>MaxSigmaS;

			std::vector<double> facetareas;//getting each facet area
			FacetIterator fi(facet_begin() );
			for(; fi < facet_end(); fi++){

				HalfedgeHandle& hh = fi->halfedge_handle_;
				FacetHandle&    fh = facet_handle(hh);
				facetareas.push_back(calc_facet_area(fh) );

			}//for

			std::cout<<"Input facet normal filtering iteration numbers (5-20��): ";
			int num;//the iteration num
			std::cin>>num;
			std::cout << "���ڵ������ȽϺ�ʱ��" << std::endl;
			std::cout << "Please wait.....  " << std::endl;

			clock_t t1 = clock();
			int i = 0;

			for(i=0; i<num; i++){

				std::vector<Normal> new_facet_normals;

				for(fi = facet_begin(); fi < facet_end(); fi++){
					HalfedgeHandle& hh = fi->halfedge_handle_;
					FacetHandle&    fh = facet_handle(hh);
					FacetHandles    NeighorFacets;
					//getNeighborFaceN1(fh,NeighorFacets); //getting facets sharing common edges
					getNeighborFaceN2(fh,NeighorFacets);//sharing common vertices

					Coord& center_fh = calc_centroid(fh);
					Normal& nf = normal(fh);
					Normal  trans(0,0,0);
					for(int j=0; j<NeighorFacets.size(); j++){

						FacetHandle& Nfh = NeighorFacets[j];
						Coord& Ncenter = calc_centroid(Nfh);
						Normal& NorN   = normal(Nfh);

						double Wc, Ws;
						Wc = exp( -(center_fh - Ncenter).sqNorm()/ (2*SigmaC*SigmaC) );
						Ws = exp( -(nf - NorN).sqNorm()          / (2*MaxSigmaS*MaxSigmaS) );

						trans += NorN * Wc*Ws*facetareas[Nfh.idx() ];


					}// end for j

					if( trans.length() > 1E-7) new_facet_normals.push_back( trans.normalize() );
					else new_facet_normals.push_back(trans);

				}//for end fi

				for(fi=facet_begin(); fi<facet_end(); fi++){

					FacetHandle fhh = facet_handle(fi->halfedge_handle_);
					fi->normal_ = new_facet_normals[fhh.idx() ];
				}
			}//for end i

			clock_t t2 = clock();
			//t2 = (t2-t1)/CLOCKS_PER_SEC;
			std::cout<<"The time of Zheng's normal filter: "<<(t2-t1)*1.0/CLOCKS_PER_SEC<<"s"<<std::endl;

	}
	///////////////////////////////////////////////////////////////
	template<class ExItems>
	void 
		ExKernelT<ExItems>::Mesh_Denoising_YouyiZheng(){//according to YouyiZheng TVCG 2011

			adjustFaceNormal_YouyiZheng();

			int vertex_num = vertex_size();
			int iterations;
			std::cout<<"Input vertex update iterations(5-30��): ";
			std::cin>>iterations;
			int i = 0;
			std::cout << "���ڵ������ȽϺ�ʱ��" << std::endl;
			std::cout << "Please wait.....  " << std::endl;
			clock_t t1 = clock();
			do{
				std::vector<Coord> updateVertexPosition;
				updateVertexPosition.resize(vertex_num);
				for(i=0; i<vertex_num; i++){
					VertexHandle vh(i);
					Coord&       vc = coord(vh);
					HalfedgeHandle& hh = halfedge_handle(vh);
					HalfedgeHandle  css(hh);
					do{
						HalfedgeHandle opp_hh = opposite_halfedge_handle(css);
						Coord&         opp_vc = coord(vertex_handle(opp_hh) );
						FacetHandle    fl = facet_handle(css);
						FacetHandle    fr = facet_handle(opp_hh);

						if(fl.is_valid() ){
							updateVertexPosition[i] += facet_ref(fl).normal_*( facet_ref(fl).normal_ *(opp_vc - vc) );

						}
						if(fr.is_valid() ){
							updateVertexPosition[i] += facet_ref(fr).normal_*( facet_ref(fr).normal_ *(opp_vc - vc) );

						}

						css = cw_rotated(css);

					}while(css != hh);
				}
				for(i=0; i<vertex_num; i++){ vertex_ref(VertexHandle(i) ).coord_ += updateVertexPosition[i]*1.0/18.0;}

			}while(--iterations);

			clock_t t2 = clock();
			//t2 = (t2-t1)/CLOCKS_PER_SEC;
			std::cout<<"The time of Zheng's vertex updating: "<<(t2-t1)*1.0/CLOCKS_PER_SEC<<"s"<<std::endl;

	}
	///////////////////////////////////////////////////////////////

} /// namespace




