#include <iostream>
#include <cassert>

struct Transformer;
struct Number;
struct BinaryOperation;
struct FunctionCall;
struct Variable;

struct Expression
{
	virtual ~Expression() {}
		virtual double evaluate() const = 0;
	virtual Expression* transform(Transformer* tr) const = 0;
};
struct Transformer //pattern Visitor
{
	virtual ~Transformer() {}
	virtual Expression* transformNumber(Number const*) = 0;
	virtual Expression* transformBinaryOperation(BinaryOperation const*) = 0;
	virtual Expression* transformFunctionCall(FunctionCall const*) = 0;
	virtual Expression* transformVariable(Variable const*) = 0;
};
struct Number : Expression
{
	Number(double value);
	double value() const;
	double evaluate() const;
	Expression* transform(Transformer* tr) const;
private:
	double value_;
};
struct BinaryOperation : Expression
{
	enum {
		PLUS = '+',
		MINUS = '-',
		DIV = '/',
		MUL = '*'
	};
	BinaryOperation(Expression const* left, int op, Expression const* right);
	~BinaryOperation();
	double evaluate() const;
	Expression* transform(Transformer* tr) const;
	Expression const* left() const;
	Expression const* right() const;
	int operation() const;
private:
	Expression const* left_;
	Expression const* right_;
	int op_;
};
struct FunctionCall : Expression
{
	FunctionCall(std::string const& name, Expression const* arg);
	~FunctionCall();
	double evaluate() const;
	Expression* transform(Transformer* tr) const;
	std::string const& name() const;
	Expression const* arg() const;
private:
	std::string const name_;
	Expression const* arg_;
};
struct Variable : Expression
{
	Variable(std::string const name);
	std::string const& name() const;
	double evaluate() const;
	Expression* transform(Transformer* tr) const;
private:
	std::string const name_;
};
/**
 * реализуйте все необходимые методы класса
 * вы можете определять любые вспомогательные
 * методы, если хотите
 */
struct CopySyntaxTree : Transformer
{
	//Создание копии узла Number
	Expression* transformNumber(Number const* number)
	{
		return new Number(number->value());
	}
	//Создание копии узла BinaryOperation
	Expression* transformBinaryOperation(BinaryOperation const* binop)
	{
		Expression* new_left = binop->left()->transform(this);
		Expression* new_right = binop->right()->transform(this);
		return new BinaryOperation(new_left, binop->operation(), new_right);
	}
	//Создание копии узла FunctionCall
	Expression* transformFunctionCall(FunctionCall const* fcall)
	{
		Expression* new_arg = fcall->arg()->transform(this);
		return new FunctionCall(fcall->name(), new_arg);
	}
	//Создание копии узла Variable
	Expression* transformVariable(Variable const* var)
	{
		return new Variable(var->name());
	}
};

struct FoldConstants : Transformer
{
	//Создание копии узла Number
	Expression* transformNumber(Number const* number) override
	{
		return new Number(number->value());
	}

	//Обработка бинарной операции. Сворачивает операнды. Если оба операнда - числа, то заменяет узел на число
	Expression* transformBinaryOperation(BinaryOperation const* binop) override
	{
		Expression* left = binop->left()->transform(this);
		Expression* right = binop->right()->transform(this);

		Number* numLeft = dynamic_cast<Number*>(left);
		Number* numRight = dynamic_cast<Number*>(right);

		if (numLeft && numRight) {
			double result;
			double l = numLeft->value();
			double r = numRight->value();
			switch (binop->operation()) {
			case BinaryOperation::PLUS:  result = l + r; break;
			case BinaryOperation::MINUS: result = l - r; break;
			case BinaryOperation::DIV:   result = l / r; break;
			case BinaryOperation::MUL:   result = l * r; break;
			default: result = 0.0;
			}
			delete left;
			delete right;
			return new Number(result);
		}
		return new BinaryOperation(left, binop->operation(), right);
	}

	//Обрабатка вызова функции. Сворачивает аргумент, и если это число, то заменяет узел на число
	Expression* transformFunctionCall(FunctionCall const* fcall) override
	{
		Expression* arg = fcall->arg()->transform(this);
		Number* numArg = dynamic_cast<Number*>(arg);

		if (numArg) {
			double result;
			double a = numArg->value();
			if (fcall->name() == "sqrt") result = sqrt(a);
			else if (fcall->name() == "abs") result = fabs(a);
			else result = 0.0;
			delete arg;
			return new Number(result);
		}
		return new FunctionCall(fcall->name(), arg);
	}

	//Создание копии
	Expression* transformVariable(Variable const* var) override
	{
		return new Variable(var->name());
	}
};


//Number: число (хранит значение)
Number::Number(double value) : value_(value) {}
double Number::value() const { return value_; }
double Number::evaluate() const { return value_; }
Expression* Number::transform(Transformer* tr) const {
	return tr->transformNumber(this);
}
//BinaryOperation: бинарная операция (+, -, *, / )
BinaryOperation::BinaryOperation(Expression const* left, int op, Expression const* right)
	: left_(left), op_(op), right_(right) {
}
BinaryOperation::~BinaryOperation() { delete left_; delete right_; }
double BinaryOperation::evaluate() const {
	double l = left_->evaluate();
	double r = right_->evaluate();
	switch (op_) {
	case BinaryOperation::PLUS: return l + r;
	case BinaryOperation::MINUS: return l - r;
	case BinaryOperation::DIV: return l / r;
	case BinaryOperation::MUL: return l * r;
	}
	return 0.0;
}
Expression* BinaryOperation::transform(Transformer* tr) const {
	return tr->transformBinaryOperation(this);
}
Expression const* BinaryOperation::left() const { return left_; }
Expression const* BinaryOperation::right() const { return right_; }
int BinaryOperation::operation() const { return op_; }

//FunctionCall: вызов функции (sqrt или abs)
FunctionCall::FunctionCall(std::string const& name, Expression const* arg)
	: name_(name), arg_(arg) {
}
FunctionCall::~FunctionCall() { delete arg_; }
double FunctionCall::evaluate() const {
	double a = arg_->evaluate();
	if (name_ == "sqrt") return sqrt(a);
	if (name_ == "abs") return fabs(a);
	return 0.0;
}
Expression* FunctionCall::transform(Transformer* tr) const {
	return tr->transformFunctionCall(this);
}
std::string const& FunctionCall::name() const { return name_; }
Expression const* FunctionCall::arg() const { return arg_; }

//Variable: переменная (значение не задаётся, evaluate() возвращает 0)
Variable::Variable(std::string const name) : name_(name) {}
std::string const& Variable::name() const { return name_; }
double Variable::evaluate() const { return 0.0; }
Expression* Variable::transform(Transformer* tr) const {
	return tr->transformVariable(this);
}


int main()
{
	setlocale(LC_ALL, "Russian");
	Number* n32 = new Number(32.0);
	Number* n16 = new Number(16.0);
	BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
	FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
	Variable* var = new Variable("var");
	BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL,
		callSqrt);
	FunctionCall* callAbs = new FunctionCall("abs", mult);
	FoldConstants FC;
	Expression* newExpr = callAbs->transform(&FC);

	std::cout << "Результат свёртки: " << newExpr->evaluate() << std::endl;
}