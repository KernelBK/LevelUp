using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

//�����㷨��
//����ƽ�нڵ����󲼾֣����ӽڵ���в��ֵ�ԭ��

/****
 *  void Layout(int lyr)
 *  {
 *     
 *  }
 * 
*****/
namespace CrowdTree
{
	class LayoutTreeLayerInfo
	{
		//�ɰ��Ų��ֵ���������
		private Dictionary<int, float> lyrLefts = new Dictionary<int,float>();
		private Dictionary<int, float> heights = new Dictionary<int,float>();

		public void Reset()
		{
			lyrLefts = new Dictionary<int, float>();
			heights = new Dictionary<int, float>();
		}

		public float GetLayoutableLeft(int level)
		{
			if (lyrLefts.ContainsKey(level))
			{
				return lyrLefts[level];
			}
			return 0;
		}

		public void SetLayoutableLeft(int level, float left)
		{
			if (lyrLefts.ContainsKey(level))
			{
				lyrLefts[level] = Math.Max(left, lyrLefts[level]);
			}
			else
			{
				lyrLefts[level] = Math.Max(0, left);
			}
		}

		public void SetLayoutLevelMinHeight(int level, float height)
		{
			if (heights.ContainsKey(level))
			{
				heights[level] = Math.Max(heights[level], height);
			}
			else
			{
				heights[level] = height;
			}
		}

		public float GetLayoutLevelMinHeight(int level)
		{
			if (heights.ContainsKey(level))
			{
				return heights[level];
			}
			return 0;
		}
	}

	class LayoutTreeNode
	{
		protected LayoutTree owner_;
		protected LayoutTreeLayerInfo lyrInfo_;
		protected int lyrLevel_;
		protected RectangleF rect_;
		protected List<LayoutTreeNode> children_ = new List<LayoutTreeNode>();

		public int Level
		{
			get { return lyrLevel_; }
			set { lyrLevel_ = value; }
		}

		public LayoutTree Owner
		{
			get { return owner_; }
			set { owner_ = value; }
		}

		public LayoutTreeLayerInfo LayerInfo
		{
			set { lyrInfo_ = value; }
		}

		public List<LayoutTreeNode> Children
		{
			get { return children_; }
		}

		public RectangleF Extent
		{
			get { return rect_; }
		}

		protected void LayoutHorizontal()
		{
			//��ȡ��ǰ�ڵ��ܹ��Ų�������λ�ã���Ԥ�Ȱ��ŵ�ǰ�ڵ㡣
			float originLeft = lyrInfo_.GetLayoutableLeft(lyrLevel_);
			rect_.X = originLeft;

			//���ݵ�ǰ�������꣬�����ӽ�㣬����Ҫ�����ӽ��İ���������µ������ڵ�İ���
			if (children_.Count > 0)
			{
				//�����ӽ��������԰��ŵ�λ��
				float childrenTotalWidth = 0.0F;
				foreach (LayoutTreeNode child in children_)
				{
					childrenTotalWidth += child.Extent.Width;
				}
				childrenTotalWidth += ((children_.Count - 1) * owner_.HorizontalSpacer);
				float childLeftest = originLeft + (rect_.Width / 2.0f) - (childrenTotalWidth / 2.0F);

				//�����ӽ�㰲�ŵ�����λ
				lyrInfo_.SetLayoutableLeft(lyrLevel_ + 1, childLeftest);

				//�����ӽ��
				for (int idxChild = 0; idxChild < children_.Count; ++idxChild)
				{
					children_[idxChild].LayoutHorizontal();
				}

				 //�����ӽ�����¶��а��õ�ǰ�ڵ�
				float center = (children_[0].Extent.Left + children_[children_.Count - 1].Extent.Right) / 2.0F;
				rect_.X = center - rect_.Width / 2.0F;
			}

			//���ýڵ��������øò������ӽ�����ܰ��ŵ�����λ�ã�������һ�µ�ǰ��Ԫ�ص����߶�
			lyrInfo_.SetLayoutableLeft(lyrLevel_, this.rect_.Right + owner_.HorizontalSpacer);
			lyrInfo_.SetLayoutLevelMinHeight(lyrLevel_, this.rect_.Height);
		}

		protected void LayoutVertical(float top)
		{
			rect_.Y = top;
			foreach (LayoutTreeNode child in children_)
			{
				child.LayoutVertical(top + lyrInfo_.GetLayoutLevelMinHeight(lyrLevel_) + owner_.VerticalSpacer);
			}
		}

		public void Layout()
		{
			LayoutHorizontal();
			LayoutVertical(0.0f);
		}

		public virtual void CalculateSize(float maxWidth, Font font, Graphics g)
		{
		}

		public virtual void CalculateSizeRecursive(float maxWidth, Font font, Graphics g)
		{
		}

		public virtual void Draw(Graphics g) { }
	}

	class TextLayoutTreeNode: LayoutTreeNode
	{
		private string text;
		private StringFormat strFmt = new StringFormat();
		private Font font;

		public String Text
		{
			get { return text; }
			set { text = value; }
		}

		public override void CalculateSize(float maxWidth, Font font, Graphics g)
		{
			strFmt.Alignment = StringAlignment.Center;
			strFmt.LineAlignment = StringAlignment.Center;

			rect_.Size = g.MeasureString(text, font, (int)maxWidth, strFmt);
			this.font = font;
		}

		public override void CalculateSizeRecursive(float maxWidth, Font font, Graphics g)
		{
			CalculateSize(maxWidth, font, g);
			foreach (LayoutTreeNode node in children_)
			{
				node.CalculateSizeRecursive(maxWidth, font, g);
			}
		}

		public override void Draw(Graphics g)
		{
			//�����������ݣ�����
			g.DrawRectangle(Pens.Black, Rectangle.Round(Extent));
			g.DrawString(text, font, Brushes.Red, Extent);

			foreach (LayoutTreeNode childNode in children_)
			{
				//����б��
				
				float childX = (childNode.Extent.Left + childNode.Extent.Right) / 2.0F;
				float childY = childNode.Extent.Top;
				
				float curX = (Extent.Left + Extent.Right) / 2.0f;
				float curY = Extent.Bottom;

				g.DrawLine(Pens.Black, new PointF(childX, childY), new PointF(curX, curY));
			}
		}
	}

	class LayoutTree
	{
		private float vertical_spacer;
		private float horizontal_spacer;

		private LayoutTreeNode root;
		private LayoutTreeLayerInfo lyrInfo = new LayoutTreeLayerInfo();

		public float VerticalSpacer
		{
			get { return vertical_spacer; }
			set { vertical_spacer = value; }
		}

		public float HorizontalSpacer
		{
			get { return horizontal_spacer; }
			set { horizontal_spacer = value; }
		}

		public LayoutTreeNode Root
		{
			get { return root; }
			set { root = value; }
		}

		public void ResetLayout()
		{
			lyrInfo.Reset();
		}

		public T CreateNode<T> (LayoutTreeNode parent) where T:LayoutTreeNode, new()
		{
			T retNode = new T();
			
			retNode.Owner = this;
			retNode.LayerInfo = this.lyrInfo;

			if (parent == null)
			{
				this.root = retNode;
				retNode.Level = 0;
			}
			else
			{
				parent.Children.Add(retNode);
				retNode.Level = parent.Level + 1;
			}
			return retNode;
		}
	}
}
