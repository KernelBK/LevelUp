#pragma once
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <string>

using namespace boost::numeric::ublas;
using namespace boost::numeric;
using namespace boost;
using namespace std;

#define printvec(a) for(int ii = 0; ii < a.size(); ++ii) { fprintf(plog, "%9.6f", a[ii]);} fprintf(plog, "\n");

//#define printvec(a)

//�ṩ�ڲ��̻߳���
class UrbanSimulatorGauss
{
public:
	UrbanSimulatorGauss(void);
	~UrbanSimulatorGauss(void);
	
	string	m_folder;	
	float	m_alpha;
	float	m_beta;
	float	m_delta;
	float	m_gamma;
	int		m_n;
	float	m_L;
	float	m_phi;
	float	m_rho;
	float	m_sigma;
	float	m_sp;
	double	m_tc;
	float	m_w0;
	long	m_grdstep;
	int		m_shortit;
	int		m_longit;

	string	m_error;

	void Init();
	void Calc();
	void CalcImpl();
	bool IsDone();
	bool StopCalc();
	float GetPercentage();
	void Reset(){}
	
private:
	ublas::vector<double> m_G, m_lambda, m_lambdan;
	matrix<double> m_D;

	ENDSTATE m_endState;	//������˳�״̬

	float	m_percentage;	//�����Ѿ�ִ�еİٷֱ�
	bool	m_isRunning;		//�߳��Ƿ���ִ��״̬
	bool	m_isDone;			//�߳��Ƿ����
	bool	m_isWaitingExit;	//�Ƿ�ȴ��˳�

	boost::mutex				m_mutex;
	boost::thread*			m_pThread;

	void SetPercentage(float v);
	void SetDone(bool v);
	void SetRunning(bool v);

	pair<bool, int> ShortEqual();
};
