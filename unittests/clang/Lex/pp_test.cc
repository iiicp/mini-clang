/***********************************
* File:     pp_test.cc
*
* Author:   蔡鹏
*
* Email:    iiicp@outlook.com
*
* Date:     2023/8/1
***********************************/

#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Basic/FileManager.h"
#include "clang/Lex/HeaderSearch.h"
#include "llvm/ADT/OwningPtr.h"
#include <gtest/gtest.h>

using namespace clang;

//===----------------------------------------------------------------------===//
// Driver PreprocessorFactory - For lazily generating preprocessors ...
//===----------------------------------------------------------------------===//

namespace {
  class  DriverPreprocessorFactory : public PreprocessorFactory {
    Diagnostic        &Diags;
    const LangOptions &LangInfo;
//    TargetInfo        &Target;
    SourceManager     &SourceMgr;
    HeaderSearch      &HeaderInfo;
public:
    DriverPreprocessorFactory(Diagnostic &diags, const LangOptions &opts,
                              /*TargetInfo &target,*/ SourceManager &SM,
                              HeaderSearch &Headers)
        : Diags(diags), LangInfo(opts), /*Target(target),*/
          SourceMgr(SM), HeaderInfo(Headers) {}


    virtual ~DriverPreprocessorFactory() {}

    virtual Preprocessor* CreatePreprocessor() override {

      if (Diags.hasErrorOccurred())
        exit(1);

      // Create the Preprocessor.
      llvm::OwningPtr<Preprocessor> PP(new Preprocessor(Diags, LangInfo, /*Target,*/
                                                        SourceMgr, HeaderInfo));

//      PreprocessorInitOptions InitOpts;
//      InitializePreprocessorInitOptions(InitOpts);
//      if (InitializePreprocessor(*PP, InitOpts))
//        return 0;

      return PP.take();
    }
  };
}


class PreprocessorTest : public testing::Test {
  private:
  llvm::OwningPtr<SourceManager> SourceMgr;
  llvm::OwningPtr<DiagnosticClient> DiagClient;
  FileManager FileMgr;

  public:
  PreprocessorTest() {

    DiagClient.reset(new TextDiagnosticPrinter(llvm::errs(),
                                               true,
                                               true,
                                               true,
                                               false,
                                               false,
                                               true,
                                               100,
                                               true));
      }

  void init(const std::string &InFile) {
    // Configure our handling of diagnostics.
    Diagnostic Diags(DiagClient.get());
    if (!SourceMgr) {
      SourceMgr.reset(new SourceManager());
    } else {
      SourceMgr->clearIDTables();
    }
    LangOptions LangInfo;
    InitializeLanguageStandard(LangInfo);
    LangInfo.setMainFileName(InFile.data());

    // Process the -I options and set them in the HeaderInfo.
    HeaderSearch HeaderInfo(FileMgr);
//    InitializeIncludePaths(argv[0], HeaderInfo, FileMgr, LangInfo);

    // Set up the preprocessor with these options.
    DriverPreprocessorFactory PPFactory(Diags, LangInfo, /**Target,*/
                                        *SourceMgr.get(), HeaderInfo);

    llvm::OwningPtr<Preprocessor> PP(PPFactory.CreatePreprocessor());

    if (!PP)
      return;

    const FileEntry *File = FileMgr.getFile(InFile);
    if (File) SourceMgr->createMainFileID(File, SourceLocation());

#if 1
    Token Tok;
     // Start preprocessing the specified input file.
    PP->EnterMainSourceFile();
    do {
      PP->Lex(Tok);
      PP->DumpToken(Tok, true);
      fprintf(stderr, "\n");
    } while (Tok.isNot(tok::eof));
#else
    SourceManager &SM = PP->getSourceManager();
    // Start lexing the specified input file.
    Lexer RawLex(SM.getMainFileID(), SM, PP.getLangOptions());
    RawLex.SetKeepWhitespaceMode(true);

    Token RawTok;
    RawLex.LexFromRawLexer(RawTok);
    while (RawTok.isNot(tok::eof)) {
      PP->DumpToken(RawTok, true);
      fprintf(stderr, "\n");
      RawLex.LexFromRawLexer(RawTok);
    }
#endif
    PP->getSourceManager().clearIDTables();

    HeaderInfo.ClearFileInfo();
    DiagClient->setLangOptions(nullptr);
  }

  private:
  static void InitializeLanguageStandard(LangOptions &Options) {
    Options.Trigraphs = 1;
    Options.BCPLComment = 1;
    Options.C99 = 1;
    Options.HexFloats = 1;
    Options.Digraphs = 1;
    Options.GNUMode = 1;
    Options.ImplicitInt = 0;
    Options.Microsoft = 1;
    Options.NoBuiltin = 0;
    Options.Bool = 0;
    Options.EmitAllDecls = 0;
    Options.OptimizeSize = 0;
    Options.GNUInline = 1;
    Options.NoInline = 1;
    Options.Static = 1;
  }
};

TEST_F(PreprocessorTest, DumpToken) {
  SCOPED_TRACE("PreprocessorTest");
  init("/Users/apple/compile/hello.c");
  ASSERT_EQ(1,1);
}