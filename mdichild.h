#ifndef CLVIEWMDICHILD_H
#define CLVIEWMDICHILD_H

#include "fx.h"
#include "FXClView.h"
#include "AceParser.h"

static FXString seqbuf;

// Main Window
class MDIChild : public FXMDIChild {
  FXDECLARE(MDIChild)
protected:
  //=== static, class-wide resources:
   static FXFont* ifont; //info font
   static short numInst;
   static FXIcon* icnDefCS;
   static FXIcon* icnBaseCS;
   static FXIcon* icnDensCS;

  // Member data
  int prevColumn;
  FXPopup* pop;
  FXVerticalFrame* contents;
  FXListBox* selcontig;
  FXTextField* selregion;
  FXComboBox* selseq;
  FXHorizontalFrame* clframe;
  FXVerticalFrame* toolframe;
  FXVerticalFrame* vframe;
  FXHorizontalFrame* hframe;
  FXLabel*            zyLabel;
  FXLabel*            zxLabel;
  FXButton*           nozoomBtn;
  FXLabel*            seqData; //length, clipping, strand
  FXTextField*        seqComment;
  FXLabel*            seqAsmInfo;
  FXLabel*          columnInfo;
  FXStatusBar*      statusbar;             // Status bar
  FXSlider* sliderZX;
  FXSlider* sliderZY;
  FXCheckButton* cbgaps;
  FXString file;
  LayoutParser* layoutparser;
  LayoutParser* groupHolder;
  struct {
     unsigned char isAce:1; //file is an .ACE file (cap3)
     unsigned char isSAM:1;
     unsigned char isPAF:1;
     unsigned char zooming:1;
     unsigned char panning:1;
     unsigned char wasMaximized:1;
  };
  double startZX, startZY;
  FXPoint zoomPt;
protected:
  void assignGroups();
  MDIChild():prevColumn(-1),pop(NULL),contents(NULL),selcontig(NULL),selregion(NULL),selseq(NULL),
		  clframe(NULL), toolframe(NULL),vframe(NULL), hframe(NULL), zyLabel(NULL), zxLabel(NULL),
		  nozoomBtn(NULL), seqData(NULL), seqComment(NULL), seqAsmInfo(NULL), columnInfo(NULL),
		  statusbar(NULL), sliderZX(NULL), sliderZY(NULL), cbgaps(NULL), layoutparser(NULL), groupHolder(NULL),
		  isAce(false), isSAM(false), isPAF(false), zooming(false), panning(false), wasMaximized(false),
		 startZX(0),startZY(0), alignview(NULL), clropt1(NULL), clropt2(NULL), clropt3(NULL){}
public:
  // Message handlers
  FXClView* alignview;
  FXOption* clropt1;
  FXOption* clropt2;
  FXOption* clropt3;

  long onCmdClZoom(FXObject*,FXSelector,void*);
  long onViewUpdate(FXObject*,FXSelector,void*);
  long onSeqSelect(FXObject*,FXSelector,void*); //clicking
  long onRMouseDown(FXObject*,FXSelector,void*);
  long onRMouseUp(FXObject*,FXSelector,void*);
  long onMMouseDown(FXObject*,FXSelector,void*);
  long onMMouseUp(FXObject*,FXSelector,void*);
  long onMouseMove(FXObject*,FXSelector,void*);
  long onNoZoom(FXObject*,FXSelector,void*);
  long onMaxZoom(FXObject*,FXSelector,void*);
  long onColorOption(FXObject*,FXSelector,void*);
  long onSelContig(FXObject*, FXSelector, void*);
  long onSelRegion(FXObject*, FXSelector, void*);
  long onHideGaps(FXObject*, FXSelector, void*);
  long onSelSeq(FXObject*, FXSelector, void*); //from list
  long onCmdGrps(FXObject*, FXSelector, void*);
  //long onMaxRestore(FXObject*, FXSelector, void*);
public:
  // Messages
  enum{
    ID_CLXZOOM=FXMDIChild::ID_LAST,
    ID_CLYZOOM,
    ID_CLNOZOOM,
    ID_CL1PXZOOM,//1pixel zoom
    ID_CSTYLE1,
    ID_CSTYLE2,
    ID_CSTYLE3,
    ID_CLVIEW,
    ID_CLGRPS,
    ID_HIDEGAPS,
    ID_SELSEQ,
    ID_MAXRESTORE,
    ID_CONTIG,
    ID_REGION
    };
public:
  MDIChild(FXMDIClient* p,const FXString& name,FXIcon* ic,FXMenuPane* mn,
                FXuint opts,FXint x,FXint y,FXint w,FXint h);
  ~MDIChild();
  virtual void create();
  void showSeqInfo(ClSeq* seq);
  FXString getFileName() { return file; }
  bool isAceFile() { return isAce; }
  bool openFile(const char* filename);
  void loadGroups(FXString& lytfile);
  bool loadLayoutFile(FXString& lytfile);
  void selContig(int ctgno, bool hideGaps);
};

#endif
