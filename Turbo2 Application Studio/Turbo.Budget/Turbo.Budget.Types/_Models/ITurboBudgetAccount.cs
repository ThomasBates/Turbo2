using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Turbo.Budget.Types
{
    public interface ITurboBudgetAccount
    {
        int AccountId { get; set; }
        string Name { get; set; }
        string Caption { get; set; }
        string Description { get; set; }
        List<ITurboBudgetTransaction> Transactions { get; }
    }
}
