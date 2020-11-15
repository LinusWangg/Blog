///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include<string>
#include "read_write.h"
namespace MeshN {
	///////////////////////////////////////////////////////////////////////////////
	// implementation
	///////////////////////////////////////////////////////////////////////////////
	template <class Mesh>
	void ReaderWriterT<Mesh>::set_mesh(Mesh* _ptr_mesh) {
		mesh_ = _ptr_mesh;
	}

	///////////////////////////////////////////////////////////////////////////////
	template <class Mesh>
	bool ReaderWriterT<Mesh>::off_reader(const char* _filename) { 
		if ( mesh_ == NULL ) { 
			std::cout<<"\nPointer to mesh is NOT defined!\n";
			exit(0);
		}

		FILE *modelfile;

		if ((modelfile = fopen( _filename, "r") ) == NULL) {
			std::cout<<"\nError: Can't open the model file!\n";
			return false;  
		}

		char letter1 = ' ',letter2=' ',letter3=' ';
		for ( ; ; ) {             // skip over comments and blank 
			if (feof(modelfile)) { // no data in file
				std::cout<<"\nError: No data in model file!\n";
				return false;  
			}

			fscanf(modelfile, "%c %c %c", &letter1,&letter2,&letter3);
			if ((letter1 == 'O')&&(letter2=='F')&&(letter3=='F')) break;  //OK!!
		} 

		int no_vertex,no_facet,no_edge;
		//std::cout<<no_vertex;

		fscanf(modelfile,"%d %d %d",&no_vertex,&no_facet,&no_edge);
		//std::cout<<no_vertex;


		float  x,  y,  z;   // components of coordinats 
		int    p0, p1, p2,p3;  // index of points
		for(int i=0;i<no_vertex;i++)

		{fscanf(modelfile, "%f %f %f", &x, &y, &z);
		mesh_->add_vertex( Coord(x, y, z) );
		}

		for(int i=0;i<no_facet;i++)

		{ fscanf(modelfile, "%d %d %d %d", &p0, &p1, &p2,&p3); //// ????????

		//mesh_->add_facet( VertexHandle(p0-1), VertexHandle(p1-1), VertexHandle(p2-1));
		mesh_->add_facet( VertexHandle(p1), VertexHandle(p2), VertexHandle(p3));

		}  // end of while

		fclose(modelfile);  

		return true;
	}


	///////////////////////////////////////////////////////////////////////////// 
	template <class Mesh>
	bool ReaderWriterT<Mesh>::ogl_writer(bool _orient, bool _smooth){
		HalfedgeHandle       cshh;
		Mesh::FacetIterator  fit (mesh_->facet_begin()); 

			//glShadeModel(GL_FLAT);
		    glShadeModel(GL_SMOOTH);
			int orient = true;// (_orient) ? 1 : -1; 
			mesh_->update_normals(); 

			for ( ; fit != mesh_->facet_end(); ++fit ) {	

				if ( (*fit).status_.is_deleted() ) continue;  

				cshh = fit->halfedge_handle_;
				FacetHandle fh = mesh_->facet_handle(cshh);
				const VertexHandle& vh0 = mesh_->vertex_handle(cshh);
				glBegin(GL_TRIANGLES); 
				do {
					const VertexHandle& vh = mesh_->vertex_handle(cshh);

					glNormal3fv( mesh_->normal(fh)*orient );
					glVertex3fv( mesh_->coord(vh) );
					cshh = mesh_->next_halfedge_handle(cshh);
				} while ( cshh != fit->halfedge_handle_ );
				glEnd();
			}

		return true;
	}


	///////////////////////////////////////////////////////////////////////////// 
	template <class Mesh>
	bool ReaderWriterT<Mesh>::ogl_writer2(bool _orient, bool _smooth){////在里面把三角面片法向画出
		HalfedgeHandle       cshh;
		Mesh::FacetIterator  fit(mesh_->facet_begin());

		//glShadeModel(GL_FLAT);
		glShadeModel(GL_SMOOTH);
		int orient = true;// (_orient) ? 1 : -1; 
		mesh_->update_normals();
		ogl_writer(_orient, _smooth);

		for (; fit != mesh_->facet_end(); ++fit) {

			if ((*fit).status_.is_deleted()) continue;

			cshh = fit->halfedge_handle_;
			FacetHandle fh = mesh_->facet_handle(cshh);
			Mesh::Normal norm = mesh_->calc_normal(fh);
			const HalfedgeHandle& hh = mesh_->halfedge_handle(fh);
			const HalfedgeHandle& p_hh = mesh_->prev_halfedge_handle(hh);
			const HalfedgeHandle& n_hh = mesh_->next_halfedge_handle(hh);

			const Coord& cd0 = mesh_->coord(mesh_->vertex_handle(hh));
			const Coord& cd1 = mesh_->coord(mesh_->vertex_handle(p_hh));
			const Coord& cd2 = mesh_->coord(mesh_->vertex_handle(n_hh));
			double gpx = (cd0.data_[0] + cd1.data_[0] + cd2.data_[0]) / 3;
			double gpy = (cd0.data_[1] + cd1.data_[1] + cd2.data_[1]) / 3;
			double gpz = (cd0.data_[2] + cd1.data_[2] + cd2.data_[2]) / 3;
			glBegin(GL_LINES);
			glVertex3f(gpx,gpy,gpz);
			glVertex3f(gpx + 0.3 * norm.x, gpy + 0.3 * norm.y, gpz + 0.3 * norm.z);
			glVertex3f(gpx + 0.3 * norm.x, gpy + 0.3 * norm.y, gpz + 0.3 * norm.z);
			glVertex3f(gpx + 0.2 * norm.x - 0.05 , gpy + 0.25 * norm.y, gpz + 0.25 * norm.z);
			glVertex3f(gpx + 0.3 * norm.x, gpy + 0.3 * norm.y, gpz + 0.3 * norm.z);
			glVertex3f(gpx + 0.2 * norm.x + 0.05, gpy + 0.25 * norm.y, gpz + 0.25 * norm.z);
			glEnd();
		}
		return true;
	}

	template <class Mesh>
	bool ReaderWriterT<Mesh>::ogl_writer3(bool _orient, bool _smooth) {////在里面把三角面片法向画出
		//Mesh::FacetIterator  fit(mesh_->facet_begin());
		std::map<VertexHandle,double> Gosk;
		mesh_->mesh_process(Gosk);
		HalfedgeHandle       cshh;
		Mesh::FacetIterator  fit(mesh_->facet_begin());
		double minn = 60000, maxn = -60000;

		for (std::map<VertexHandle, double>::iterator it = Gosk.begin(); it != Gosk.end(); it++)
		{
			if (it->second < minn)
				minn = it->second;
			if (it->second > maxn)
				maxn = it->second;
		}

		double dx = maxn - minn;
		double inter = dx / 5;

		//glShadeModel(GL_FLAT);
		glShadeModel(GL_SMOOTH);
		int orient = true;// (_orient) ? 1 : -1; 
		mesh_->update_normals();

		for (; fit != mesh_->facet_end(); ++fit) {

			if ((*fit).status_.is_deleted()) continue;

			cshh = fit->halfedge_handle_;
			FacetHandle fh = mesh_->facet_handle(cshh);
			const VertexHandle& vh0 = mesh_->vertex_handle(cshh);
			double in = 80.0 / 20.0;
			glBegin(GL_TRIANGLES);
			do {
				const VertexHandle& vh = mesh_->vertex_handle(cshh);
				for (int i = 0; i < 19; i++)
				{
					if (Gosk[vh] > 40.0)
						glColor3f(1.0, 0.3, 0.0);
					else if (Gosk[vh] < -40.0)
						glColor3f(0.0, 0.3, 1.0);
					else if (Gosk[vh] < 40 - in * i && Gosk[vh]>40 - in * (i + 1))
						glColor3f(1.0 - (i + 1) * 0.05, 0.3, 0.0 + (i + 1) * 0.05);
				}
				
				glNormal3fv(mesh_->normal(fh) * orient);
				glVertex3fv(mesh_->coord(vh));
				cshh = mesh_->next_halfedge_handle(cshh);
			} while (cshh != fit->halfedge_handle_);
			glEnd();
		}

		return true;
	}
	
} //namespace
