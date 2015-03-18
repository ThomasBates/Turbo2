using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Turbo.Budget.SQLCE
{
    interface ITurboBudgetSQLCEAccountAdapter
    {
        BudgetAccount BudgetAccount { get; }
    }
}
