#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <stack>

using namespace llvm;

namespace
{
struct SimpleMath : public FunctionPass
{
  static char ID;
  SimpleMath() : FunctionPass(ID) {}

  bool doInitialization(Module &M)
  {
    return false;
  }

  void printOperand(Value *op)
  {
    if (isa<Instruction>(*op))
    {
      Instruction *_I = cast<Instruction>(op);
      errs() << *_I << "\n";
    }
    else if (isa<ConstantInt>(*op))
    {
      ConstantInt *_V = cast<ConstantInt>(op);
      errs() << std::to_string(_V->getSExtValue()) << "\n";
    }
    else
      errs() << op << "\n";
  }

  bool isConstant(Value *op)
  {
    if (isa<ConstantInt>(*op))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  bool replaceConstants(Function &F)
  {
    bool changed = false;
    std::stack<Instruction *> s;

    for (BasicBlock &BB : F)
    {
      for (Instruction &I : BB)
      {
        s.push(&I);
      }
    }

    while (!s.empty())
    {
      Instruction *I = s.top();
      s.pop();
      Value *left;
      Value *right;
      switch (I->getOpcode())
      {
      case Instruction::Add:
        left = I->getOperand(0);
        right = I->getOperand(1);
        if (isConstant(left) && isConstant(right))
        {
          ConstantInt *VL = cast<ConstantInt>(left);
          ConstantInt *VR = cast<ConstantInt>(right);
          int result = VL->getZExtValue() + VR->getZExtValue();
          ConstantInt *constResult = ConstantInt::get(F.getContext(), APInt(/*nbits*/ 32, result, /*bool*/ true));
          I->replaceAllUsesWith(constResult);
          I->eraseFromParent();
          errs() << "Changed\n"
                 << result;
          changed = true;
        }
        break;
      case Instruction::Sub:
        left = I->getOperand(0);
        right = I->getOperand(1);
        if (isConstant(left) && isConstant(right))
        {
          ConstantInt *VL = cast<ConstantInt>(left);
          ConstantInt *VR = cast<ConstantInt>(right);
          int result = VL->getZExtValue() - VR->getZExtValue();
          ConstantInt *constResult = ConstantInt::get(F.getContext(), APInt(/*nbits*/ 32, result, /*bool*/ true));
          I->replaceAllUsesWith(constResult);
          I->eraseFromParent();
          errs() << "Changed "
                 << constResult->getZExtValue() << " \n";
          changed = true;
        }
        break;
      case Instruction::Mul:
        left = I->getOperand(0);
        right = I->getOperand(1);
        if (isConstant(left) && isConstant(right))
        {
          ConstantInt *VL = cast<ConstantInt>(left);
          ConstantInt *VR = cast<ConstantInt>(right);
          int result = VL->getZExtValue() * VR->getZExtValue();
          ConstantInt *constResult = ConstantInt::get(F.getContext(), APInt(/*nbits*/ 32, result, /*bool*/ true));
          I->replaceAllUsesWith(constResult);
          I->eraseFromParent();
          errs() << "Changed\n"
                 << result;
          changed = true;
        }
        break;
      case Instruction::SDiv:
        left = I->getOperand(0);
        right = I->getOperand(1);
        if (isConstant(left) && isConstant(right))
        {
          ConstantInt *VL = cast<ConstantInt>(left);
          ConstantInt *VR = cast<ConstantInt>(right);
          int result = VL->getZExtValue() / VR->getZExtValue();
          ConstantInt *constResult = ConstantInt::get(F.getContext(), APInt(/*nbits*/ 32, result, /*bool*/ true));
          I->replaceAllUsesWith(constResult);
          I->eraseFromParent();
          errs() << "Changed\n"
                 << result;
          changed = true;
        }
        break;
      default:
        break;
      }
    }

    return changed;
  }

  int getInstructionNumber(Function &F, Instruction *I2)
  {
    int numInst = 0;

    for (BasicBlock &BB : F)
    {
      for (Instruction &I : BB)
      {
        if (I.isIdenticalToWhenDefined(I2)) {
          return numInst;
        }
        numInst++;
      }
    }

    return numInst;
  }

  void printTemps(Function &F)
  {
    int numInst = 0;

    for (BasicBlock &BB : F)
    {
      for (Instruction &I : BB)
      {
        errs() << "I" << numInst << ":   " << I;
        bool hasUses = false;
        for (auto U : I.users())
        { // U is of type User*
          hasUses = true;
          if (auto I2 = dyn_cast<Instruction>(U))
          {
            errs() << " | I" << getInstructionNumber(F, I2) << ":   " << *I2;
          }
        }
        if (!hasUses)
        {
          errs() << " | I" << numInst << ":   " << I;
        }
        errs() << "\n";
        numInst++;
      }
    }
  }

  bool runOnFunction(Function &F)
  {
    printTemps(F);
    while (replaceConstants(F))
      ;

    return true;
  }

  bool doFinalization(Module &M)
  {
    return false;
  }
};
} // namespace

char SimpleMath::ID = 0;
static RegisterPass<SimpleMath> X("sm", "Simple math pass", false, false);
