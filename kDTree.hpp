#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
struct kDTreeNode
{
    vector<int> data;
    int label;
    kDTreeNode* left;
    kDTreeNode* right;
    kDTreeNode(vector<int> data, kDTreeNode* left = nullptr, kDTreeNode* right = nullptr)
    {
        this->data = data;
        this->label = 0;
        this->left = left;
        this->right = right;
    }
    kDTreeNode(vector<int> data, int label, kDTreeNode* left = nullptr, kDTreeNode* right = nullptr)
    {
        this->data = data;
        this->label = label;
        this->left = nullptr;
        this->right = nullptr;
    }

    friend ostream& operator<<(ostream& os, const kDTreeNode& node)
    {
        os << "(";
        for (int i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
};

class kDTree
{
private:
    int k;
    kDTreeNode* root;
    int count;

private:
    void SOS(kDTreeNode* node, vector<kDTreeNode*>& nodes)
    {
        if (!node)
            return;

        SOS(node->left, nodes);
        nodes.push_back(node);
        SOS(node->right, nodes);
    }
    kDTreeNode* copyTree(const kDTreeNode* node)
    {
        if (!node)
            return nullptr;

        kDTreeNode* newNode = new kDTreeNode(node->data);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);

        return newNode;
    }
    int getHeight(const kDTreeNode* node) const
    {

        if (!node)
            return 0;

        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);

        // Chi?u cao c?a cây là chi?u cao l?n nh?t gi?a cây con trái và cây con ph?i, c?ng thêm 1 cho nút hi?n t?i
        return 1 + max(leftHeight, rightHeight);
    }
    int countLeaf(const kDTreeNode* node) const
    {
        if (!node)
            return 0;
        if (!node->left && !node->right)
            return 1;
        return countLeaf(node->left) + countLeaf(node->right);
    }
    void inorder(const kDTreeNode* node) const
    {
        if (node)
        {
            inorder(node->left);
            cout << *node << " ";
            inorder(node->right);
        }
    }
    void preorder(const kDTreeNode* node) const
    {
        if (node)
        {
            cout << *node << " ";
            preorder(node->left);
            preorder(node->right);
        }
    }
    void postorder(const kDTreeNode* node) const
    {
        if (node)
        {
            postorder(node->left);
            postorder(node->right);
            cout << *node << " ";
        }
    }
    kDTreeNode* helpInsert(kDTreeNode* node, const vector<int>& point, int level)
    {
        if (!node)
            return new kDTreeNode(point);

        if (point.size() != k)
            return node;

        int alpha = level % k;
        if (node->data[alpha] > point[alpha])
        {
            node->left = helpInsert(node->left, point, level + 1);
        }
        else
        {
            node->right = helpInsert(node->right, point, level + 1);
        }

        return node;
    }
    bool helpSearch(kDTreeNode* node, const vector<int>& point, int level)
    {
        if (node == nullptr)
        {
            return false;
        }
        int alpha = level % (this->k);
        if (node->data == point)
        {
            return true;
        }
        else if (node->data[alpha] > point[alpha])
        {
            return helpSearch(node->left, point, level + 1);
        }
        else
        {
            return helpSearch(node->right, point, level + 1);
        }
    }
    void merge(vector<vector<int>>& arr, int left, int mid, int right, int alpha)
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        vector<vector<int>> L(n1), R(n2);
        for (int i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[mid + 1 + j];
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2)
        {
            if (L[i][alpha] <= R[j][alpha])
            {
                arr[k] = L[i];
                i++;
            }
            else
            {
                arr[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1)
        {
            arr[k] = L[i];
            i++;
            k++;
        }
        while (j < n2)
        {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    void mergeSort(vector<vector<int>>& arr, int left, int right, int alpha)
    {
        if (left >= right)
            return;
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, alpha);
        mergeSort(arr, mid + 1, right, alpha);
        merge(arr, left, mid, right, alpha);
    }
    
    void merge(vector<kDTreeNode*>& bestList, int left, int mid, int right, const vector<int>& target)
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        vector<kDTreeNode*> L(n1), R(n2);
        for (int i = 0; i < n1; i++)
            L[i] = bestList[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = bestList[mid + 1 + j];
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2)
        {
            if (distance(L[i]->data, target) <= distance(R[j]->data, target))
            {
                bestList[k] = L[i];
                i++;
            }
            else
            {
                bestList[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1)
        {
            bestList[k] = L[i];
            i++;
            k++;
        }
        while (j < n2)
        {
            bestList[k] = R[j];
            j++;
            k++;
        }
    }

    void mergeSort(vector<kDTreeNode*>& bestList, int left, int right, const vector<int>& target)
    {
        if (left >= right)
            return;
        int mid = left + (right - left) / 2;
        mergeSort(bestList, left, mid, target);
        mergeSort(bestList, mid + 1, right, target);
        merge(bestList, left, mid, right, target);
    }
    kDTreeNode* helpBuildTree(const vector<vector<int>>& pointList, vector<int>& label, int start, int end, int level)
    {
        if (start > end)
        {
            return nullptr;
        }

        int alpha = level % k;
        mergeSortLabel(const_cast<vector<vector<int>> &>(pointList), label, start, end, alpha);

        int mid = start + (end - start) / 2;
        vector<int> medianVec = pointList[mid];

        for (int i = start; i <= mid; i++)
        {
            if (pointList[i][alpha] == medianVec[alpha])
            {
                medianVec = pointList[i];
                mid = i;
                break;
            }
        }

        kDTreeNode* newNode = new kDTreeNode(medianVec, label[mid]);
        this->count++;

        newNode->left = helpBuildTree(pointList, label, start, mid - 1, level + 1);
        newNode->right = helpBuildTree(pointList, label, mid + 1, end, level + 1);

        return newNode;
    }
    kDTreeNode* helpBuildTree(const vector<vector<int>>& pointList, int start, int end, int level)
    {
        if (start > end)
        {
            return nullptr;
        }

        int alpha = level % k;
        mergeSort(const_cast<vector<vector<int>> &>(pointList), start, end, alpha);

        int mid = start + (end - start) / 2;
        vector<int> medianVec = pointList[mid];

        for (int i = start; i <= mid; i++)
        {
            if (pointList[i][alpha] == medianVec[alpha])
            {
                medianVec = pointList[i];
                mid = i;
                break;
            }
        }

        kDTreeNode* newNode = new kDTreeNode(medianVec);
        this->count++;

        newNode->left = helpBuildTree(pointList, start, mid - 1, level + 1);
        newNode->right = helpBuildTree(pointList, mid + 1, end, level + 1);

        return newNode;
    }
    kDTreeNode* findMin(kDTreeNode* node, int alpha, int level)
    {
        if (!node)
        {
            return nullptr;
        }
        int newAlpha = level % k;
        if (newAlpha == alpha)
        {
            if (node->left == nullptr)
            {
                return node;
            }
            else
            {
                return findMin(node->left, alpha, level + 1);
            }
        }
        else
        {
            kDTreeNode* nodeLeft = findMin(node->left, alpha, level + 1);
            kDTreeNode* nodeRight = findMin(node->right, alpha, level + 1);

            kDTreeNode* nodeMin = node;
            if (nodeLeft && nodeLeft->data[alpha] < nodeMin->data[alpha])
            {
                nodeMin = nodeLeft;
            }
            if (nodeRight && nodeRight->data[alpha] < nodeMin->data[alpha])
            {
                nodeMin = nodeRight;
            }
            return nodeMin;
        }
    }
    void helpRemove(kDTreeNode*& node, const vector<int>& point, int level)
    {
        if (!node)
            return;
        int alpha = level % this->k;
        if (node->data == point)
        {
            if (node->right)
            {
                kDTreeNode* nodeMin = findMin(node->right, alpha, level + 1);
                node->data = nodeMin->data;
                helpRemove(node->right, nodeMin->data, level + 1);
            }
            else if (node->left)
            {
                kDTreeNode* nodeMin = findMin(node->left, alpha, level + 1);
                node->data = nodeMin->data;
                node->right = node->left;
                node->left = nullptr;
                helpRemove(node->right, nodeMin->data, level + 1);
            }
            else
            {
                delete node;
                node = nullptr;
            }
        }
        else
        {
            if (point[alpha] < node->data[alpha])
            {
                helpRemove(node->left, point, level + 1);
            }
            else
            {
                helpRemove(node->right, point, level + 1);
            }
        }
    }
    double distance(vector<int> x1, vector<int> x2)
    {
        double distance = 0;
        for (int i = 0; i < this->k; i++)
        {
            // distance += pow((x1[i] - x2[i]), 2);
            distance += (x1[i] - x2[i]) * (x1[i] - x2[i]);
        }
        return sqrt(distance);
    }
    void helpNearestNeighbour(kDTreeNode* node, const vector<int>& target, kDTreeNode*& best, int level)
    {
        if (!node)
        {
            return;
        }
        int alpha = level % this->k;

        if (target[alpha] < node->data[alpha])
        {
            helpNearestNeighbour(node->left, target, best, level + 1);
            if (!best)
            {
                best = node;
            }
        }
        else
        {
            helpNearestNeighbour(node->right, target, best, level + 1);
            if (!best)
            {
                best = node;
            }
        }
        int d = abs(target[alpha] - node->data[alpha]);
        double R = distance(target, best->data);
        double r = distance(target, node->data);
        if (r < R)
        {
            best = node;
            R = r;
        }

        if (d < R)
        {
            if (target[alpha] < node->data[alpha])
            {
                helpNearestNeighbour(node->right, target, best, level + 1);
            }
            else
            {
                helpNearestNeighbour(node->left, target, best, level + 1);
            }
        }
    }
    void helpKNearestNeighbour(kDTreeNode* node, const vector<int>& target, int k, vector<kDTreeNode*>& bestList, int level)
    {
        if (k >= this->count)
        {
            SOS(node, bestList);
            return;
        }
        if (!node)
            return;
        int alpha = level % this->k;
        kDTreeNode* other;
        if (target[alpha] < node->data[alpha])
        {
            helpKNearestNeighbour(node->left, target, k, bestList, level + 1);
        }
        else
        {
            helpKNearestNeighbour(node->right, target, k, bestList, level + 1);
        }

        if (bestList.size() < k)
        {
            bestList.push_back(node);
            if (target[alpha] < node->data[alpha])
            {
                helpKNearestNeighbour(node->right, target, k, bestList, level + 1);
            }
            else
            {
                helpKNearestNeighbour(node->left, target, k, bestList, level + 1);
            }
        }
        else
        {
            double d = abs(target[alpha] - node->data[alpha]);
            double maxDis = 0;
            int index = 0;
            double R = distance(node->data, target);
            for (int i = 0; i < bestList.size(); i++)
            {
                if (maxDis <= distance(target, bestList[i]->data))
                {
                    maxDis = distance(target, bestList[i]->data);
                    index = i;
                }
            }

            if (R < maxDis)
            {
                bestList.erase(bestList.begin() + index);
                bestList.push_back(node);
            }

            if (d < maxDis)
            {
                if (target[alpha] < node->data[alpha])
                {
                    helpKNearestNeighbour(node->right, target, k, bestList, level + 1);
                }
                else
                {
                    helpKNearestNeighbour(node->left, target, k, bestList, level + 1);
                }
            }
        }
    }
    void mergeLabel(vector<vector<int>>& arr, vector<int>& label, int left, int mid, int right, int alpha)
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        vector<vector<int>> L(n1);
        vector<int> L_label(n1);
        vector<vector<int>> R(n2);
        vector<int> R_label(n2);

        for (int i = 0; i < n1; i++)
        {
            L[i] = arr[left + i];
            L_label[i] = label[left + i];
        }
        for (int j = 0; j < n2; j++)
        {
            R[j] = arr[mid + 1 + j];
            R_label[j] = label[mid + 1 + j];
        }

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2)
        {
            if (L[i][alpha] <= R[j][alpha])
            {
                arr[k] = L[i];
                label[k] = L_label[i];
                i++;
            }
            else
            {
                arr[k] = R[j];
                label[k] = R_label[j];
                j++;
            }
            k++;
        }

        while (i < n1)
        {
            arr[k] = L[i];
            label[k] = L_label[i];
            i++;
            k++;
        }

        while (j < n2)
        {
            arr[k] = R[j];
            label[k] = R_label[j];
            j++;
            k++;
        }
    }
    void mergeSortLabel(vector<vector<int>>& arr, vector<int>& label, int left, int right, int alpha)
    {
        if (left >= right)
            return;

        int mid = left + (right - left) / 2;
        mergeSortLabel(arr, label, left, mid, alpha);
        mergeSortLabel(arr, label, mid + 1, right, alpha);
        mergeLabel(arr, label, left, mid, right, alpha);
    }

public:
    kDTree(int k = 2)
    {
        this->k = k;
        this->root = nullptr;
        this->count = 0;
    }
    ~kDTree()
    {
        this->count = 0;
    }
    const kDTree& operator=(const kDTree& other)
    {
        this->k = other.k;
        this->count = other.count;
        this->root = this->copyTree(other.root);
        return *this;
    }
    kDTree(const kDTree& other)
    {
        this->k = other.k;
        this->count = other.count;
        this->root = this->copyTree(other.root);
    }

    int nodeCount() const
    {
        return this->count;
    }
    int height() const
    {
        return this->getHeight(this->root);
    }
    int leafCount() const
    {
        return this->countLeaf(this->root);
    }
    void inorderTraversal() const
    {
        this->inorder(this->root);
    }
    void preorderTraversal() const
    {
        this->preorder(this->root);
    }
    void postorderTraversal() const
    {
        this->postorder(this->root);
    }
    void insert(const vector<int>& point)
    {
        this->root = this->helpInsert(this->root, point, 0);
        this->count++;
    }
    void remove(const vector<int>& point)
    {
        if (this->search(point))
        {
            this->helpRemove(this->root, point, 0);
            this->count--;
        }
    }
    bool search(const vector<int>& point)
    {
        return this->helpSearch(this->root, point, 0);
    }
    void buildTree(const vector<vector<int>>& pointList, const vector<int>& label)
    {
        this->root = helpBuildTree(pointList, const_cast<vector<int> &>(label), 0, pointList.size() - 1, 0);
    }
    void nearestNeighbour(const vector<int>& target, kDTreeNode*& best)
    {
        best = nullptr;
        if (this->search(target))
        {
            best = new kDTreeNode(target);
            return;
        }
        this->helpNearestNeighbour(this->root, target, best, 0);
    }
    void kNearestNeighbour(const vector<int>& target, int k, vector<kDTreeNode*>& bestList)
    {
        bestList.clear();
        this->helpKNearestNeighbour(this->root, target, k, bestList, 0);
        mergeSort(bestList, 0, bestList.size() - 1, target);
    }
    void buildTree(const vector<vector<int>>& pointList)
    {
        this->root = helpBuildTree(pointList, 0, pointList.size() - 1, 0);
    }
    friend class kNN;
};

class kNN
{
private:
    int k;
    Dataset* X_train;
    Dataset* y_train;
    kDTree kdtree;

public:
    kNN(int k = 5) : k(k)
    {
    }
    void fit(Dataset& X_train, Dataset& y_train)
    {
        this->X_train = &X_train;
        this->y_train = &y_train;
        if (X_train.data.size())
        {
            int dimentsion = X_train.data.front().size();
            kdtree.k = dimentsion;
            vector<vector<int>> pointList;
            vector<int> label;
            for (const auto& point : X_train.data)
            {
                vector<int> pointData(point.begin(), point.end());
                pointList.push_back(pointData);
            }
            for (const auto& row : y_train.data)
            {
                label.push_back(row.front());
            }
            kdtree.buildTree(pointList, label);
        }
    }
    Dataset predict(Dataset& X_test)
    {
        Dataset result;
        result.columnName.push_back("label");
        for (const auto& point : X_test.data)
        {
            vector<int> target(point.begin(), point.end());
            vector<kDTreeNode*> bestList;
            kdtree.kNearestNeighbour(target, this->k, bestList);
            vector<int> visit(10, 0);
            for (int i = 0; i < bestList.size(); i++)
            {
                visit[bestList[i]->label]++;
            }
            int max = 0;
            int tempLabel = -1;
            for (int i = 0; i < 10; i++)
            {
                if (visit[i] > max)
                {
                    max = visit[i];
                    tempLabel = i;
                }
            }
            list<int> res;
            res.push_back(tempLabel);
            result.data.push_back(res);
        }
        return result;
    }
    double score(const Dataset& y_test, const Dataset& y_pred)
    {
        int count = 0;
        if (y_pred.data.size() != y_test.data.size() || y_pred.data.size() <= 0 || y_test.data.size() <= 0)
        {
            return -1;
        }
        auto it1 = y_test.data.begin();
        auto it2 = y_pred.data.begin();
        while (it1 != y_test.data.end())
        {
            auto label1 = (*it1).front();
            auto label2 = (*it2).front();

            if (label1 == label2)
            {
                count++;
            }
            ++it1;
            ++it2;
        }

        return count * 1.0 / y_test.data.size();
    }
};

// Please add more or modify as needed
