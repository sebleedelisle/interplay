#include "ofxEtherdream.h"

//--------------------------------------------------------------
void ofxEtherdream::setup() {
    etherdream_lib_start();
    
    setPPS(30000);
    setWaitBeforeSend(false);
    
	cout << "ETHERDREAM SET UP -----------------------------------------------" << endl;
	
	state = ETHERDREAM_WAITING;
	waitStartMils = ofGetElapsedTimeMillis(); 
	recheckDelay = 0;

	start();
}


//--------------------------------------------------------------
//bool ofxEtherdream::stateIsFound() {
//    return state == ETHERDREAM_FOUND;
//}

string ofxEtherdream::getStateString() {
	switch (state) {
		case ETHERDREAM_IDLE :
			return "ETHERDREAM_IDLE";
			break;
		case ETHERDREAM_WAITING :
			return "ETHERDREAM_WAITING";
			break;
		case ETHERDREAM_CONNECTION_FAILED :
			return "ETHERDREAM_CONNECTION_FAILED";
			break;
		case ETHERDREAM_RUNNING :
			return "ETHERDREAM_RUNNING";
			break;
		case ETHERDREAM_DISCONNECTED :
			return "ETHERDREAM_DISCONNECTED";
			break;
	}
	
	return "error";
	
}

/*
//--------------------------------------------------------------
bool ofxEtherdream::checkConnection(bool bForceReconnect) {
 
	//cout << "ETHERDREAM CHECK CONNECTION -----------------------------------------------" << endl;

	if(device->state == ST_SHUTDOWN) {
		state = ETHERDREAM_NOTFOUND;
        if(bForceReconnect) {
			cout << "ETHERDREAM FORCE RECONNECT -----------------------------------------------" << endl;

            kill();
            setup();
        }
        return false;
    }
    return true;
}
*/
//--------------------------------------------------------------
void ofxEtherdream::init() {
	cout << "ETHERDREAM INIT -----------------------------------------------" << endl;
    int device_num = etherdream_dac_count();

	cout << "FOUND " << device_num << " DEVICE(S)" << endl;
	if (!device_num) {
		ofLogWarning() << "ofxEtherdream::init - No DACs found";
		state = ETHERDREAM_CONNECTION_FAILED;

		return 0;
	}
    
	for (int i=0; i<device_num; i++) {
		ofLogNotice() << "ofxEtherdream::init - " << i << " Ether Dream " << etherdream_get_id(etherdream_get(i));

	}
    
	device = etherdream_get(0);
    
	ofLogNotice() << "ofxEtherdream::init - Connecting...";
	if (etherdream_connect(device) < 0) {
		state = ETHERDREAM_CONNECTION_FAILED;
		return 1;
	}
    
    ofLogNotice() << "ofxEtherdream::init - done";
    
    state = ETHERDREAM_RUNNING;
}


string ofxEtherdream :: getDeviceStateString() {
	
	if(device== NULL) return "No devices connected";
	if(device->state == ST_DISCONNECTED)
		return "Disconnected";
	else if(device->state == ST_READY)
		return "Ready";
	else if(device->state == ST_RUNNING)
		return "Running";
	else if(device->state == ST_BROKEN)
		return "Broken!";
	else if(device->state == ST_SHUTDOWN)
		return "Shut down";
	else
		return "Unknown state";
	
	
	
	
}

//--------------------------------------------------------------
void ofxEtherdream::threadedFunction() {
    while (isThreadRunning() != 0) {
        
        switch (state) {
            case ETHERDREAM_WAITING:
				
				if(lock()) {
					recheckDelay --; 
					if(recheckDelay<0){
						
						//cout << ofGetElapsedTimeMillis() - waitStartMils << endl;
						if (etherdream_dac_count()>0) {
							init();
						}
						// if it's been more than 2 seconds
						if(ofGetElapsedTimeMillis() - waitStartMils > 1000){
							state = ETHERDREAM_CONNECTION_FAILED;
						}						
						recheckDelay = 1000000;
					}
							
                    unlock();
                }
	
                break;

            case ETHERDREAM_RUNNING:
                if(lock()) {
					this->blocking = true;
					if((device==NULL) || (device->state == ST_DISCONNECTED) || (device->state == ST_BROKEN) || (device->state == ST_SHUTDOWN)) {
						state = ETHERDREAM_DISCONNECTED;
					} else {
						send();
					}
                    unlock();
                }
                break;
			default:
				break;
			
        }
    }
}

//--------------------------------------------------------------
void ofxEtherdream::start() {
    startThread(false, false);  // TODO: blocking or nonblocking?
}

//--------------------------------------------------------------
void ofxEtherdream::stop() {
	
    stopThread();
}

//--------------------------------------------------------------
void ofxEtherdream::send() {
//    if(!stateIsFound() || points.empty()) return;
	
	if((points.empty()) || (device==NULL) || (device->state == ST_DISCONNECTED) || (device->state == ST_BROKEN) || (device->state == ST_SHUTDOWN)) return;
  
	//cout << "ETHERDREAM SENDING POINTS " << points.size() << endl;
    
	if(bWaitBeforeSend) etherdream_wait_for_ready(device);
    else if(!etherdream_is_ready(device)) return;
	/*
    dataBuffer.clear();
	// fill the buffer with something important
	for(int i = 0; i<points.size(); i++) {
		ofxIlda::Point& p = points[i];
		dataBuffer.append(ofToString(p.x)+ " " );
		dataBuffer.append(ofToString(p.y)+ " " );
		dataBuffer.append(ofToString(p.r)+ " " );
		dataBuffer.append(ofToString(p.g)+ " " );
		dataBuffer.append(ofToString(p.b)+ " " );
		dataBuffer.append(ofToString(p.a)+ " " );
		dataBuffer.append(ofToString(p.u1)+ " " );
		dataBuffer.append(ofToString(p.u2)+ "\n" );
	}*/
	
	//bool fileWritten = ofBufferToFile("etherdream.txt", dataBuffer);
	//
	
    // DODGY HACK: casting ofxIlda::Point* to etherdream_point*
    int res = etherdream_write(device, (etherdream_point*)points.data(), points.size(), pps, -1);
    if (res != 0) {
        ofLogVerbose() << "ofxEtherdream::write " << res;
    }
    points.clear();
}


//--------------------------------------------------------------
void ofxEtherdream::clear() {
    if(lock()) {
        points.clear();
        unlock();
    }
}

//--------------------------------------------------------------
void ofxEtherdream::addPoints(const vector<ofxIlda::Point>& _points) {
    if(lock()) {
        if(!_points.empty()) {
            points.insert(points.end(), _points.begin(), _points.end());
        }
        unlock();
    }
}


//--------------------------------------------------------------
void ofxEtherdream::addPoints(const ofxIlda::Frame &ildaFrame) {
    addPoints(ildaFrame.getPoints());
}


//--------------------------------------------------------------
void ofxEtherdream::setPoints(const vector<ofxIlda::Point>& _points) {
    if(lock()) {
        points = _points;
        unlock();
    }
}


//--------------------------------------------------------------
void ofxEtherdream::setPoints(const ofxIlda::Frame &ildaFrame) {
    setPoints(ildaFrame.getPoints());
}

//--------------------------------------------------------------
void ofxEtherdream::setWaitBeforeSend(bool b) {
    if(lock()) {
        bWaitBeforeSend = b;
        unlock();
    }
}

//--------------------------------------------------------------
bool ofxEtherdream::getWaitBeforeSend() const {
    return bWaitBeforeSend;
}


//--------------------------------------------------------------
void ofxEtherdream::setPPS(int i) {
    if(lock()) {
        pps = i;
        unlock();
    }
}

//--------------------------------------------------------------
int ofxEtherdream::getPPS() const {
    return pps;
}
