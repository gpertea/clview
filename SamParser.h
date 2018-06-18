#ifndef SAMPARSER_H
#define SAMPARSER_H
#include "LayoutParser.h"
#include "htslib/hfile.h"
#include "htslib/bgzf.h"
#include "htslib/hts.h"
#include "htslib/sam.h"

class SamParser : public LayoutParser {
 protected:
  virtual LytSeqInfo* addSeq(char* s, LytCtgData* ctg);
  char* readSeq(LytSeqInfo* seqinfo=NULL); //assumes the next line is just sequence data
                   //reads everything until after the next empty line
  bam_hdr_t* header;
  hts_idx_t *bamidx;
  samFile* bamf;
  bool hasIndex;
 public:
  SamParser(const char* filename):LayoutParser(filename), header(NULL), bamidx(NULL),
         bamf(NULL), hasIndex(false) {}
  virtual bool open();
  virtual ~SamParser() {
	  if (bamidx) hts_idx_destroy(bamidx);
	  if (header) bam_hdr_destroy(header);
	  if (bamf) sam_close(bamf);
  }
  int64_t sam_seek(int64_t ofs) {
	if (bamf==NULL) GError("Error: cannot call sam_tell() with NULL samFile!\n");
	if (bamf->is_bgzf)
		return bgzf_seek(bamf->bgzf, ofs, SEEK_SET);
	//if (bamf->is_cram) -- add CRAM support?
	off_t fpos(ofs);
	if (bamf->format.format==sam)
		return (int64_t)hseek(bamf->hfile, fpos, SEEK_SET);
	GError("Error: unrecognized file format? sam_seek()\n");
	return -1;
  }

  int64_t sam_tell(int64_t ofs) {
	  if (bamf==NULL) GError("Error: cannot call sam_tell() with NULL samFile!\n");
	  if (bamf->is_bgzf)
		  return bgzf_tell(bamf->bgzf);
	  //if (bamf->is_cram)
	  if (bamf->format.format==sam)
		  return (int64_t) htell(bamf->hfile);
	  GError("Error: unrecognized file format? sam_tell()\n");
	  return -1;
  }
  virtual bool parse(fnLytSeq* seqfn=NULL); //load all the file offsets
  virtual bool parseContigs(); //load contigs' file offsets
  virtual bool loadContig(int ctgidx, fnLytSeq* seqfn=NULL,
                     bool re_pos=true); //for loading by browsing
  //sequence loading - only by request
  virtual char getFileType() { return 'S'; }
  virtual char* getSeq(LytSeqInfo* seqinfo);
  virtual char* getContigSeq(LytCtgData* ctgdata);
};

#endif
