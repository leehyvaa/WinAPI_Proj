#pragma once
struct Node
{
    double g;
    double h;
    double f;

    POINT pos;
    POINT parent;
};
class PriorityQueue
{
private:
    vector<Node> arr;
public:
    PriorityQueue();
    ~PriorityQueue();
    void Push(Node node);
    Node Pop();
    int GetCount();
    
};

/*
�����̽��ٸ� ������ �÷��̾ playerRail���� ���� �� �ְ� �ǰ� �̶�
���ʹ������� ���� ���ϰ� �ϴ� ��� 2����
enemyZone���� ���� ������ ���� �浹üũ�ؼ� ������,
���� Ȧ������ üũ�ؼ� ������

�÷��̾ tempRail�� �׸���.
tempRail�� �׸��� ������ �ǽð����� �����ǥ ~������ȯ�� �� ~ �÷��̾� ��ġ ������ ����ؼ�
LineTO�� �׸�
playerRail���� �����̽��ٸ� ������ enemyZone���� ������
������ ��ȯ�Ҷ����� ���� ���ÿ��� �����Ѵ�. ���� �����̽��ٸ� �� ���
������ ���� �ٽ� ���ư���. �� ���ư��� tempRail �ʱ�ȭ
���� ���ƴٴϴ� PlayerRail �� �ٽ� �����ϸ� enemyZone�̶� PlayerRail�� �ٽ� �׷��� �ϴµ�
�� �� ����� - ������ ������ ������ �ҽǵȴ�.
���⼭ �ΰ��� ������ ����� ���µ� �ҽ������ �߰������� �̾��� A����(�ҽǷ���)��
���� ���Ͽ��� �ҽ����� �� ���� + �߰����� B����(����)�� �����.


�ҽǷ����� ������ ���� ���Ͽ��� ������� ������ ������ ���� ���� �����Ѵ�
����� - ������ - ������ ������ �����ϰ� ���⼭ ���� �߰��� ������ �߰��Ѵ�.
����� ��������Ʈ�� �����ؾ� ���ҵ�

������ ������ ���� ���Ͽ��� ��������� ������ ���� ������ ������ �� �� ���̿� ���� �߰���
������ �߰��Ѵ�
������ ���� - ����� - ���� �׸� �� - ������ - ������ ���� ������ �̾����


���̼ҽ����̶� ���ϰ� ���丷 �������� �����ϸ�
���� ��ũ�� ����Ʈ ���Ͽ��� �÷��̾ ������ �� ���� ���������� ���� ���� ����� ����
�÷��̾ �����̽��ٷ� ���� �� ������� �������� �¿���(A,B)�� �̰�
���������� ���� �����Ǿ��ִ� �¿� ��(C,D)�� �̰�
�� ���� �����Ҷ����� A������ CȤ�� D�� ���� ������ +�� -�� �� ������ ���� ���丷����(Q)1��
B������ CȤ�� D�� ���������� ���� �ݴ�� �� ������ ���� ���丷����(W) 1���� �����

���� Q���� �ϼ�������
Q���� - ����� - ���� ���� ���� - ������
W���ϵ� ���� �����ϴ�


���ؾ� �� �� ����� ����� ���� �¿� ��
������ ����� ���� �¿��� �̴�.
����, �¿�� �ݺ����� ���� 2���� �������� ������ �̸� �¿����̶� ������

*/