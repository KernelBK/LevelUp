using System;
using System.Collections.Generic;
using System.Text;

using Sharp3D.Math;
using Sharp3D.Math.Core;

namespace Mesh2D
{
	public class Bone
	{
		private string m_name;

		//�����ı�������ϵ��һ�����һ����ת������ѣ�
		private Matrix3D m_mat;

		//������ȫ������ϵ�����԰ѹ����ռ�����Ľڵ�任���ֲ��ռ�����
		private Matrix3D m_totalMat;

		//������λ��
		private Vector2D m_refPosition;

		public Bone(string name)
		{
			m_refPosition = new Vector2D(0.0, 0.0);
			m_mat = Matrix3D.Identity.Clone();
			m_totalMat = Matrix3D.Identity.Clone();
			m_name = name;
		}

		public string Name
		{
			get { return m_name; }
			set { m_name = value; }
		}

		public Sharp3D.Math.Core.Vector2D RefPosition
		{
			get { return m_refPosition; }
			set { m_refPosition = value; }
		}

		public Vector2D GetRelPositon(Vector2D pos)
		{
			return Vector2D.Subtract(pos, m_refPosition);
		}

		//����������ϵ�еĶ������ù����任���ֲ�����ϵ��
		public Vector2D TransformPosition(Vector2D refPos)
		{
			//�����������ϵ�е�����
			Vector2D relPos = GetRelPositon(refPos);
			Vector3D homoPos = new Vector3D(relPos.X, relPos.Y, 1.0);
			
			//�ӹ�������ϵ�任���ֲ�����ϵ
			Vector3D TransformedPos = Matrix3D.Transform(m_totalMat, homoPos);
			return new Vector2D(TransformedPos.X, TransformedPos.Y);
		}

		//����������ȫ�ֱ任���󴫵ݵ��ӹ����ϡ�
		public void Update(Bone parent)
		{
			if(parent == null)
			{
				m_totalMat = m_mat;
				return;
			}

			Vector2D refPosDiff = m_refPosition - parent.m_refPosition;

			//transform and then rotate
			Matrix3D transMat = Matrix.Transform2D(refPosDiff.X, refPosDiff.Y);

			m_totalMat = Matrix3D.Multiply(parent.m_totalMat, transMat);

			//contact rotate matrix
			m_totalMat = Matrix3D.Multiply(m_totalMat, m_mat);
		}

		public void SetLocalMatrix(Matrix3D mat3D)
		{
			m_mat = mat3D.Clone();
		}

		public void ContactLocalMatrixRight(Matrix3D mat3D)
		{
			m_mat = Matrix3D.Multiply(m_mat, mat3D);
		}

		public void ContactLocalMatrixLeft(Matrix3D mat3D)
		{
			m_mat = Matrix3D.Multiply(mat3D, m_mat);
		}
	}

	public class Skeleton
	{
		private Skeleton m_parent;
		private Bone m_bone;
		private IList<Skeleton> m_children;

		public Mesh2D.Bone Bone
		{
			get { return m_bone; }
			set { m_bone = value; }
		}

		public Mesh2D.Skeleton Parent
		{
			get { return m_parent; }
			set { m_parent = value; }
		}

		public IList<Skeleton> Children
		{
			get { return m_children; }
		}

		public Skeleton()
		{
			m_parent = null;
			m_bone = new Bone("tmp");
			m_children = null;
		}

		public Skeleton(Bone bone, Skeleton parent)
		{
			m_bone = bone;
			m_parent = parent;
			m_children = null;

			if (m_parent != null)
			{
				m_parent.AddChild(this);
			}
		}

		public void UpdateDeep()
		{
			this.UpdateShallow();
			if(m_children != null)
			{
				foreach(Skeleton child in m_children)
				{
					child.UpdateDeep();
				}
			}
		}

		public void UpdateShallow()
		{
			if (m_parent != null)
			{
				m_bone.Update(m_parent.m_bone);
			}
			else
			{
				m_bone.Update(null);
			}
		}

		public IList<Skeleton> GetChildren()
		{
			return m_children;
		}

		public void AddChild(Skeleton child)
		{
			if (m_children == null)
			{
				m_children = new List<Skeleton>();
			}
			m_children.Add(child);
			child.m_parent = this;
		}

		public Skeleton GetChild(int index)
		{
			if (m_children == null)
			{
				return null;
			}
			return m_children[index];
		}

		public void RemoveChildAt(int index)
		{
			if (m_children == null) return;
			m_children.RemoveAt(index);
		}

		public void RemoveChild(Skeleton child)
		{
			if (m_children == null) return;
			m_children.Remove(child);
		}
	}

	public class Vertex
	{
		private Vector2D m_refPosition;
		private IList<Bone> m_bones;
		private IList<double> m_weights;

		public Vertex(Vector2D v, IList<Bone> bones, IList<double> weights)
		{
			m_refPosition = v.Clone();
			m_bones = bones;
			m_weights = weights;
		}

		//ûɶ��˵�ģ���Ȩ��Ͼ�OK�ˡ�
		public Vector2D GetTransformedPositon()
		{
			Vector2D retPos = new Vector2D(0.0, 0.0);

			int nBones = m_bones.Count;
			for(int iBone = 0; iBone < nBones; ++iBone)
			{
				retPos += m_bones[iBone].TransformPosition(m_refPosition) * m_weights[iBone];
			}

			return retPos;
		}
	}
}
