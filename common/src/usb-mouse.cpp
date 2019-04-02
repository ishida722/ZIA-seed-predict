/*
 *  Copyright 2018 Digital Media Professionals Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <usb-mouse.h>

using namespace dmp;
using namespace util;

#if UINTPTR_MAX == 0xffffffffffffffffULL
# define BUILD_64   1
#endif

#ifdef BUILD_64
#define WORD_SIZE 64
#else
#define WORD_SIZE 32
#endif

#define ERR(...) fprintf(stderr, __VA_ARGS__); fflush(stderr)

static unsigned int SCREEN_RESOLUTION = 96; //DPI
static unsigned int MOUSE_REFRESH_RATE = 125; // Hz
static unsigned int MOUSE_RESOLUTION = 400; //DPI
static unsigned int SCREEN_WIDTH = 1280; //Pixel
static unsigned int SCREEN_HEIGHT = 720; //Pixel

unsigned int mouse_cursor[11] = {0x40,0x60,0x70,0x78,0x7C,0x7E,0x7F,0x78,0x68,0x4C,0x04}; //7x11

unsigned int* textbuf11 = NULL;  //[SCREEN_W*11];

static int fid = 0;
string SEARCH_DIR = "/sys/class/input";
static string _mouse_event;
static string _mouse_name;

// find files in a specific folder
int getdir (string dir, vector<string> &files)
{
  DIR *dp;
  struct dirent *dirp;
  if((dp  = opendir(dir.c_str())) == NULL) {
    ERR("Could not open %s\n", dir.c_str());
    return errno;
  }

  while ((dirp = readdir(dp)) != NULL) {
    files.push_back(string(dirp->d_name));
  }
  closedir(dp);
  return 0;
}

// create mask
bool* create_mask( int length){
  bool *ret = NULL;
  for( int i = 0; i < length; i++)
    ret[ i] = false;
  return ret;
}

// set value for the mask
void set_mask( int intval, int startidx, int wordsize, bool* & mask){
  for( int i = startidx; i < startidx+wordsize; i++){
    if ((1 << (i-startidx)) & intval)
      mask[ i] = true;
    else
      mask[ i] = false;
  }
}

//only process in words, so masks must be a multiple
int get_mask_length( int max_bit){
  if (max_bit <= 0)
    return WORD_SIZE;
  else
    return ((max_bit/WORD_SIZE) + 1) * WORD_SIZE;
}

// Split a data by token
vector<string> split(string data, string token)
{
  vector<string> output;
  size_t pos = string::npos; // size_t to avoid improbable overflow
  do
  {
    pos = data.find(token);
    output.push_back(data.substr(0, pos));
    if (string::npos != pos)
      data = data.substr(pos + token.size());
  } while (string::npos != pos);
  return output;
}

class Device{
private:
  string _path = "";
  bool* _evmask;
  bool* _absmask;
  bool* _relmask;
  bool* _keymask;
public:
  Device( string path){
  	_path = path;
    _evmask = new bool[ get_mask_length( EV_MAX)];
    _absmask = new bool[ get_mask_length( ABS_MAX)];
    _relmask = new bool[ get_mask_length( REL_MAX)];
    _keymask = new bool[ get_mask_length( KEY_MAX)];
  }

  string path(){
  	return _path;
  }

  string name(){
    string name_path = _path + "/device/name";
    string nm_temp;
    struct stat st;
    if( stat( name_path.c_str(), &st) == 0){
       ifstream ifs( name_path);
       string nm( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>() ) );
       nm_temp = nm;
       nm_temp.erase(std::remove(nm_temp.begin(), nm_temp.end(), '\n'), nm_temp.end());
    }
    return nm_temp;
  }

  string dev_path(){
    string temp = _path.substr( SEARCH_DIR.size());
    return ( "/dev/input" + temp);
  }

  //algorithm from test_pointers() in udev-builtin-input_id.c in udev
  bool is_mouse(){
    if (_keymask[ BTN_TOOL_FINGER] == true){
      //logging.debug("found a touchpad, ignoring");
      return false;
    }
    if ( _keymask[ BTN_MOUSE] == true){
      //ABS stuff seems to be for VMWare's mouse according to comments
      //in the C code, leaving here in case
      if ( _evmask[ EV_ABS] == true){
        if ((_absmask[ ABS_X] == true) && ( _absmask[ ABS_Y] == true))
          return true;
      }
      //for a real actual mouse
      if ( _evmask[ EV_REL] == true){
        if (( _relmask[ REL_X] == true) && ( _relmask[ REL_Y] == true))
          return true;
      }
    }
    return false;
  }

  void _build_cap_mask( string val, string capname){
    bool *mask = NULL;
    if ( capname == "ev"){
      mask = _evmask;
    }
    else if ( capname == "rel"){
      mask = _relmask;
    }
    else if ( capname == "abs"){
      mask = _absmask;
    }
    else if ( capname == "key"){
      mask = _keymask;
    }
    //this shouldn't happen, here for sanity check
    else{
      mask = NULL;
    }

    auto vals = split( val, " ");
    int i = vals.size() - 1;
    int j = 0;
    //walk backwards due to endianness
    while ( i >= 0){
      int64_t intval = strtol( vals[i].c_str(), nullptr, 16);
      set_mask( intval, j*WORD_SIZE, WORD_SIZE, mask);
      i -= 1;
      j += 1;
    }
  }

  void read_capabilities(){
    //This should be called after you create the Device object so that
    // the capabilities are read from /sys"""
    string cap_dir = _path + "/device/capabilities";
    struct stat st;
    if( stat( cap_dir.c_str(), &st) == 0){
      vector<string> evts = vector<string>();
      getdir( cap_dir, evts);
      for ( unsigned i = 0; i < evts.size(); i++) {
        if( (evts[i] == "ev") || (evts[i] == "abs")|| (evts[i] == "rel") || (evts[i] == "key")){
          string cap = evts[i];
          string cap_path = cap_dir + "/" + cap;
          ifstream ifs( cap_path);
          string val( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>() ) );
          this->_build_cap_mask( val, cap);
        }
      }
    }
  }
};

// Scan through existing events to find which one is from mouse
void scan4mouse( string &event_path, string &mouse_name){

  vector<string> files = vector<string>();
  getdir( SEARCH_DIR, files);
  string mouse_evt;
  string mouse_nm;
  for( unsigned i = 0; i < files.size(); i++){
    string str = files[i];
    string substr = "event";
    if( strncmp( str.c_str(), substr.c_str(), substr.size()) == 0){
      Device dev_temp( SEARCH_DIR + "/" + str);
      dev_temp.read_capabilities();
      if( dev_temp.is_mouse() == true){
        mouse_evt = "/dev/input/" + str;
        mouse_nm = dev_temp.name();
      }
    }
  }
  event_path = mouse_evt;
  mouse_name = mouse_nm;
}

// The events of external devices getting through USB gates are packed into "event" in "/dev/input" folder
// Go to the folder to check which event is the mouse and change the mouse event
void set_mouse_event( string me){
  _mouse_event = me;
}

// Update mouse infor if necessary
void mouse_config(unsigned int m_refresh_rate, unsigned int m_resolution) {
  MOUSE_REFRESH_RATE = m_refresh_rate;
  MOUSE_RESOLUTION = m_resolution;
}

// Update screen infor if necessary
void screen_config(unsigned int screen_width, unsigned int screen_height, unsigned int screen_resolution){
  SCREEN_WIDTH = screen_width;
  SCREEN_HEIGHT = screen_height;
  SCREEN_RESOLUTION = screen_resolution;
}

// Init function for mouse
void mouse_init(mouse_infor &mouse_infor, unsigned int mousex, unsigned int mousey){
  textbuf11 = new unsigned int[SCREEN_WIDTH * 11];
  mouse_infor.cur.x = mousex;
  mouse_infor.cur.y = mousey;
  mouse_infor.l_press.x = 0;
  mouse_infor.l_press.y = 0;
  mouse_infor.m_press.x = 0;
  mouse_infor.m_press.y = 0;
  mouse_infor.r_press.x = 0;
  mouse_infor.r_press.y = 0;
  mouse_infor.l_pressed = false;
  mouse_infor.m_pressed = false;
  mouse_infor.r_pressed = false;
  scan4mouse( _mouse_event, _mouse_name);
}

// Open mouse event
// Return 0 -> success, 1 -> fail
int mouse_open(){

  const char *mouse_event = _mouse_event.c_str();
  const char *mouse_name = _mouse_name.c_str();
  // Events from the mouse are packed into a file
  fid = open( mouse_event, O_NONBLOCK);// use O_RDONLY if you want a non-stop reading input event until something is read
  if (fid == 0){
    ERR("Could not open %s!\n", mouse_event);
    return EXIT_FAILURE;
  }
  else if (fid == -1){
    ERR ("%s is not a valid device!\n", mouse_event );
    return EXIT_FAILURE;
  }

  //Print Device Name
  //ioctl (fid, EVIOCGNAME (sizeof (name)), name);
  ERR ("%s (%s) is opened!\n", mouse_event, mouse_name);

  return EXIT_SUCCESS;
}

// Close mouse
// Return 0 --> Success; 1 --> Fail
int mouse_close(){
  if( close(fid) == EXIT_SUCCESS){
    return EXIT_SUCCESS;
  }
  else{
    return EXIT_FAILURE;
  }
}

// Read events that come up in the input file
int mouse_read(mouse_infor &m_infor){ //// Event type from <linux/input.h>
  unsigned int nbytes;
  input_event event;
  nbytes = read(fid, &event, sizeof(event));
  if (nbytes >= sizeof(event)) {
    switch (event.type){
      case EV_KEY:
        switch (event.code){
        case BTN_LEFT:
          m_infor.l_press.x = m_infor.cur.x;
          m_infor.l_press.y = m_infor.cur.y;
          m_infor.l_press.w = m_infor.cur.w;
          if (event.value == 1) {m_infor.l_pressed = true;}
          else{ m_infor.l_pressed = false;}
          break;
        case BTN_RIGHT:
          m_infor.r_press.x = m_infor.cur.x;
          m_infor.r_press.y = m_infor.cur.y;
          m_infor.r_press.w = m_infor.cur.w;
          if (event.value == 1) {m_infor.r_pressed = true;}
          else{ m_infor.r_pressed = false;}
          break;
        case BTN_MIDDLE:
          m_infor.m_press.x = m_infor.cur.x;
          m_infor.m_press.y = m_infor.cur.y;
          m_infor.m_press.w = m_infor.cur.w;
          if (event.value == 1) {m_infor.m_pressed = true;}
          else{ m_infor.m_pressed = false;}
          break;
        }
        break;
      case EV_REL:
        switch (event.code){
        case REL_X:
          m_infor.cur.x += event.value;
          //m_infor.cur.x += mouse2pointer(event.value);
          m_infor.cur.x = m_infor.cur.x < 0 ? 0 : m_infor.cur.x;
          m_infor.cur.x = m_infor.cur.x > static_cast<int>( SCREEN_WIDTH - 1) ? SCREEN_WIDTH - 1: m_infor.cur.x;
          break;
        case REL_Y:
          m_infor.cur.y += event.value;
          //m_infor.cur.y += mouse2pointer(event.value);
          m_infor.cur.y = m_infor.cur.y < 0? 0: m_infor.cur.y;
          m_infor.cur.y = m_infor.cur.y > static_cast<int>( SCREEN_HEIGHT - 1) ? SCREEN_HEIGHT - 1: m_infor.cur.y;
          break;
        case REL_WHEEL:
          m_infor.cur.w += event.value;
          break;
        }
    }
  }
  return nbytes;
}

