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
    std::stack<Instruction*> s;

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
          ConstantInt *constResult = ConstantInt::get(F.getContext(), APInt(/*nbits*/32, result, /*bool*/true));
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
          ConstantInt *constResult = ConstantInt::get(F.getContext(), APInt(/*nbits*/32, result, /*bool*/true));
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
          ConstantInt *constResult = ConstantInt::get(F.getContext(), APInt(/*nbits*/32, result, /*bool*/true));
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
          ConstantInt *constResult = ConstantInt::get(F.getContext(), APInt(/*nbits*/32, result, /*bool*/true));
          I->replaceAllUsesWith(constResult);
          I->eraseFromParent();
          errs() << "Changed\n"
                 << result;
          changed = true;
        }
        break;
      default:
        errs() << "Instruction isn't arithmetic\n";
        break;
      }
    }

    return changed;
  }

  bool runOnFunction(Function &F)
  {
    while(replaceConstants(F));

    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        Value *left;
        Value *right;
        switch (I.getOpcode()) {
          case Instruction::Add:
          case Instruction::Sub:
          case Instruction::Mul:
          case Instruction::SDiv:
            left = I.getOperand(0);
            right = I.getOperand(1);
            isConstant(left);
            isConstant(right);
            errs() << "Instruction: " << I << "\n";
            errs() << "    Left operand: ";
            printOperand(left);
            errs() << "    Right operand: ";
            printOperand(right);
            break;
          default:
            errs() << "Instruction isn't arithmetic\n";
            break;
        }
      }
    }
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
