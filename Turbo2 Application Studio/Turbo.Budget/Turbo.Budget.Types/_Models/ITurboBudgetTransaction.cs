using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Turbo.Budget.Types
{
    public interface ITurboBudgetTransaction
    {
        int ID { get; set; }
        DateTime TimeStamp { get; set; }
        string Description { get; set; }
        ITurboBudgetAccount Account { get; set; }
        ITurboBudgetAccount OtherAccount { get; set; }
        decimal Amount { get; set; }
    }
}
