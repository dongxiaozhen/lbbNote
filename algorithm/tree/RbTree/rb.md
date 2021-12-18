# 红黑树 https://github.com/HuKeping/rbtree/blob/master/rbtree.go

##删除(中心思想：一边的黑色节点比另一边少，如果旁边有红色就借一个)
```go
	for x != t.root && x.Color == BLACK {
		if x == x.Parent.Left {
			w := x.Parent.Right
			if w.Color == RED { // case 1
				w.Color = BLACK
				x.Parent.Color = RED
				t.leftRotate(x.Parent)
				w = x.Parent.Right
			}
			if w.Left.Color == BLACK && w.Right.Color == BLACK { // case 2
				w.Color = RED
				x = x.Parent
			} else {
				if w.Right.Color == BLACK { // case 3
					w.Left.Color = BLACK
					w.Color = RED
					t.rightRotate(w)
					w = x.Parent.Right
				}
				w.Color = x.Parent.Color
				x.Parent.Color = BLACK
				w.Right.Color = BLACK
				t.leftRotate(x.Parent)
				// this is to exit while loop
				x = t.root
			}
        }else{
            // ...  右分支
        }
    }
```go
				/*
				 * Case 1 - left rotate at parent
				 *
				 *     P               S
				 *    / \             / \
				 *   N   s    -->    p   Sr
				 *      / \         / \
				 *     Sl  Sr      N   Sl
				 */
// case1 -> case 2 把兄弟节点弄成黑色

				/*
				 * Case 2 - sibling color flip
				 * (p could be either color here)
				 *
				 *    (p)           (p)
				 *    / \           / \
				 *   N   S    -->  N   s
				 *      / \           / \
				 *     Sl  Sr        Sl  Sr
				 *
// 如果p是红色，设置成黑色，退出循环,(一边多了一个黑色，一边少了一个黑色，平衡了) 

				 * Case 3 - right rotate at sibling
				 * (p could be either color here)
				 *
				 *   (p)           (p)
				 *   / \           / \
				 *  N   S    -->  N   Sl
				 *     / \             \
				 *    sl  Sr            s
				 *                       \
				 *                        Sr
				 *
// case3 -> case4 把兄弟节点的右孩子弄成红色

    			/*
    			 * Case 4 - left rotate at parent + color flips
    			 * (p and sl could be either color here.
    			 *  After rotation, p becomes black, s acquires
    			 *  p's color, and sl keeps its color)
    			 *
    			 *      (p)             (s)
    			 *      / \             / \
    			 *     N   S     -->   P   Sr
    			 *        / \         / \
    			 *      (sl) sr      N  (sl)
    			 */


### 只有case2 的状态下，p 是黑色需要循环，其他情况都能自修复平衡，(其他情况可以从旁边的节点借一个红色设置成黑色，但是如果都是黑色的情况下没有办法借到)


##添加(中心思想：一边多了一个红色(黑色变的)，往另一边移动一个红色)
				/*
				 * Case 1 - node's uncle is red (color flips).
				 *
				 *       G            g
				 *      / \          / \
				 *     p   u  -->   P   U
				 *    /            /
				 *   n            n

				/*
				 * Case 2 - node's uncle is black and node is
				 * the parent's right child (left rotate at parent).
				 *
				 *      G             G
				 *     / \           / \
				 *    p   U  -->    n   U
				 *     \           /
				 *      n         p
				 *
// case2 -> case3

            /*
			 * Case 3 - node's uncle is black and node is
			 * the parent's left child (right rotate at gparent).
			 *
			 *        G           P
			 *       / \         / \
			 *      p   U  -->  n   g
			 *     /                 \
			 *    n                   U
			 */ 

