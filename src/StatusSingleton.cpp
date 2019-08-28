#include <DatCustomUtils/Tensorflow/StatusSingleton.hpp>


TFStatusSingleton::TFStatusSingleton(): pStatus(TF_NewStatus()){
}

TFStatusSingleton& TFStatusSingleton::instance(){
	static TFStatusSingleton singleton;
	return singleton;
}


TF_Status* TFStatusSingleton::getStatus() const{
	return pStatus;
}

TFStatusSingleton::~TFStatusSingleton(){
}

TFStatusSingleton& TFStatusSingleton::operator=(const TFStatusSingleton& rhs) { return rhs.instance(); }
