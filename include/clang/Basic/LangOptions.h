//===--- LangOptions.h - C Language Family Language Options -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the LangOptions interface.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LANGOPTIONS_H
#define LLVM_CLANG_LANGOPTIONS_H

namespace clang {

  /// LangOptions - This class keeps track of the various options that can be
  /// enabled, which controls the dialect of C that is accepted.
  class LangOptions {
public:
    unsigned Trigraphs : 1;      // Trigraphs in source files.
    unsigned BCPLComment : 1;    // BCPL-style '//' comments.
    unsigned Bool : 1;           // 'bool', 'true', 'false' keywords.
    unsigned DollarIdents : 1;   // '$' allowed in identifiers.
    unsigned AsmPreprocessor : 1;// Preprocessor in asm mode.
    unsigned GNUMode : 1;        // True in gnu99 mode false in c99 mode (etc)
    unsigned ImplicitInt : 1;    // C89 implicit 'int'.
    unsigned Digraphs : 1;       // C94, C99 and C++
    unsigned HexFloats : 1;      // C99 Hexadecimal float constants.
    unsigned C99 : 1;            // C99 Support
    unsigned Microsoft : 1;      // Microsoft extensions.

    unsigned NoBuiltin : 1;   // Do not use builtin functions (-fno-builtin)
                              // by locks.
    unsigned EmitAllDecls : 1;// Emit all declarations, even if
                              // they are unused.

    unsigned Optimize : 1;    // Whether __OPTIMIZE__ should be defined.
    unsigned OptimizeSize : 1;// Whether __OPTIMIZE_SIZE__ should be
                              // defined.
    unsigned Static : 1;      // Should __STATIC__ be defined (as
                              // opposed to __DYNAMIC__).

    unsigned GNUInline : 1;// Should GNU inline semantics be
                           // used (instead of C99 semantics).
    unsigned NoInline : 1; // Should __NO_INLINE__ be defined.

private:
    unsigned SymbolVisibility : 3;// Symbol's visibility.

    /// The user provided name for the "main file", if non-null. This is
    /// useful in situations where the input file name does not match
    /// the original input file, for example with -save-temps.
    const char *MainFileName;

public:
    unsigned InstantiationDepth;// Maximum template instantiation depth.


    enum VisibilityMode {
      Default,
      Protected,
      Hidden
    };

    LangOptions() {
      Trigraphs = BCPLComment = Bool = DollarIdents = AsmPreprocessor = 0;
      GNUMode = ImplicitInt = Digraphs = 0;
      HexFloats = 0;
      C99 = Microsoft = 0;
      NoBuiltin = 0;
      SymbolVisibility = (unsigned) Default;

      EmitAllDecls = 0;

      InstantiationDepth = 99;

      Optimize = 0;
      OptimizeSize = 0;

      Static = 0;

      GNUInline = 0;
      NoInline = 0;

      MainFileName = 0;
    }

    const char *getMainFileName() const { return MainFileName; }
    void setMainFileName(const char *Name) { MainFileName = Name; }

    VisibilityMode getVisibilityMode() const {
      return (VisibilityMode) SymbolVisibility;
    }
    void setVisibilityMode(VisibilityMode v) { SymbolVisibility = (unsigned) v; }
  };

}// namespace clang

#endif
